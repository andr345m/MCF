// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#include "heap_dbg.h"
#include "../ext/unref_param.h"

typedef int Dummy;

#ifdef __MCF_CRT_HEAPDBG_ON

#include "bail.h"
#include "mcfwin.h"
#include "../ext/strcpyout.h"
#include <wchar.h>

typedef __MCF_HeapDbgBlockInfo BlockInfo;

static bool BlockInfoComparatorNodes(const MCF_AvlNodeHeader *pInfo1, const MCF_AvlNodeHeader *pInfo2){
	return (uintptr_t)(((const BlockInfo *)pInfo1)->pContents) <
		(uintptr_t)(((const BlockInfo *)pInfo2)->pContents);
}
static bool BlockInfoComparatorNodeKey(const MCF_AvlNodeHeader *pInfo1, intptr_t nKey2){
	return (uintptr_t)(((const BlockInfo *)pInfo1)->pContents) < (uintptr_t)(void *)nKey2;
}
static bool BlockInfoComparatorKeyNode(intptr_t nKey1, const MCF_AvlNodeHeader *pInfo2){
	return (uintptr_t)(void *)nKey1 < (uintptr_t)(((const BlockInfo *)pInfo2)->pContents);
}

#define GUARD_BAND_SIZE	0x20u

static HANDLE		g_hMapAllocator;
static MCF_AvlRoot	g_pavlBlocks;

bool __MCF_CRT_HeapDbgInit(){
	g_hMapAllocator = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
	if(!g_hMapAllocator){
		return false;
	}
	return true;
}
void __MCF_CRT_HeapDbgUninit(){
	const BlockInfo *pBlockInfo = (const BlockInfo *)g_pavlBlocks;
	if(pBlockInfo){
		for(;;){
			const BlockInfo *const pPrev = (const BlockInfo *)MCF_AvlPrev((const MCF_AvlNodeHeader *)pBlockInfo);
			if(!pPrev){
				break;
			}
			pBlockInfo = pPrev;
		}

		MCF_CRT_Bail(L"__MCF_CRT_HeapDbgUninit() 失败：侦测到内存泄漏。\n\n"
			"如果您选择调试应用程序，MCF CRT 将尝试使用 OutputDebugString() 导出内存泄漏的详细信息。");

		wchar_t awcBuffer[256];
		do {
			const unsigned char *pbyDump = pBlockInfo->pContents;
			wchar_t *pwcWrite = awcBuffer + __mingw_snwprintf(
				awcBuffer, sizeof(awcBuffer) / sizeof(wchar_t),
				L"地址 %0*zX  大小 %0*zX  调用返回地址 %0*zX  首字节 ",
				(int)(sizeof(size_t) * 2), (size_t)pbyDump,
				(int)(sizeof(size_t) * 2), (pBlockInfo->uSize),
				(int)(sizeof(size_t) * 2), (size_t)(pBlockInfo->pRetAddr));
			for(size_t i = 0; i < 16; ++i){
				*(pwcWrite++) = L' ';
				if(IsBadReadPtr(pbyDump, 1)){
					*(pwcWrite++) = L'?';
					*(pwcWrite++) = L'?';
				} else {
					static const wchar_t HEX_TABLE[16] = L"0123456789ABCDEF";
					*(pwcWrite++) = HEX_TABLE[*pbyDump >> 4];
					*(pwcWrite++) = HEX_TABLE[*pbyDump & 0x0F];
				}
				++pbyDump;
			}
			*pwcWrite = 0;

			OutputDebugStringW(awcBuffer);

			pBlockInfo = (const BlockInfo *)MCF_AvlNext((const MCF_AvlNodeHeader *)pBlockInfo);
		} while(pBlockInfo);

		__asm__ __volatile__("int 3 \n");
	}

	g_pavlBlocks = NULL;
	HeapDestroy(g_hMapAllocator);
	g_hMapAllocator = NULL;
}

size_t __MCF_CRT_HeapDbgGetRawSize(size_t uContentSize){
	return uContentSize + GUARD_BAND_SIZE * 2;
}
unsigned char *__MCF_CRT_HeapDbgAddGuardsAndRegister(
	unsigned char *pRaw, size_t uContentSize, const void *pRetAddr)
{
	unsigned char *const pContents = pRaw + GUARD_BAND_SIZE;

	void **ppGuard1 = (void **)pContents;
	void **ppGuard2 = (void **)(pContents + uContentSize);
	for(unsigned i = 0; i < GUARD_BAND_SIZE; i += sizeof(void *)){
		--ppGuard1;

		*ppGuard1 = EncodePointer(ppGuard2);
		*ppGuard2 = EncodePointer(ppGuard1);

		++ppGuard2;
	}

	BlockInfo *const pBlockInfo = HeapAlloc(g_hMapAllocator, 0, sizeof(BlockInfo));
	if(!pBlockInfo){
		MCF_CRT_BailF(L"__MCF_CRT_HeapDbgAddGuardsAndRegister() 失败：内存不足。\n调用返回地址：%0*zX",
			(int)(sizeof(size_t) * 2), (size_t)pRetAddr);
	}
	pBlockInfo->pContents	= pContents;
	pBlockInfo->uSize		= uContentSize;
	pBlockInfo->pRetAddr	= pRetAddr;
	MCF_AvlAttach(&g_pavlBlocks, (MCF_AvlNodeHeader *)pBlockInfo, &BlockInfoComparatorNodes);

	return pContents;
}
const BlockInfo *__MCF_CRT_HeapDbgValidate(
	unsigned char **ppRaw, unsigned char *pContents, const void *pRetAddr)
{
	unsigned char *const pRaw = pContents - GUARD_BAND_SIZE;
	*ppRaw = pRaw;

	const BlockInfo *const pBlockInfo = (const BlockInfo *)MCF_AvlFind(
		&g_pavlBlocks, (intptr_t)pContents, &BlockInfoComparatorNodeKey, &BlockInfoComparatorKeyNode);
	if(!pBlockInfo){
		MCF_CRT_BailF(L"__MCF_CRT_HeapDbgValidate() 失败：传入的指针无效。\n调用返回地址：%0*zX",
			(int)(sizeof(size_t) * 2), (size_t)pRetAddr);
	}

	void *const *ppGuard1 = (void *const *)pContents;
	void *const *ppGuard2 = (void *const *)(pContents + pBlockInfo->uSize);
	for(unsigned i = 0; i < GUARD_BAND_SIZE; i += sizeof(void *)){
		--ppGuard1;

		if((DecodePointer(*ppGuard1) != ppGuard2) || (DecodePointer(*ppGuard2) != ppGuard1)){
			MCF_CRT_BailF(L"__MCF_CRT_HeapDbgValidate() 失败：侦测到堆损坏。\n调用返回地址：%0*zX",
				(int)(sizeof(size_t) * 2), (size_t)pRetAddr);
		}

		++ppGuard2;
	}

	return pBlockInfo;
}
void __MCF_CRT_HeapDbgUnregister(const BlockInfo *pBlockInfo){
	MCF_AvlDetach((const MCF_AvlNodeHeader *)pBlockInfo);
	HeapFree(g_hMapAllocator, 0, (void *)pBlockInfo);
}

#endif // __MCF_CRT_HEAPDBG_ON
