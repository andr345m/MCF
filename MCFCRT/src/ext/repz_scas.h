// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_EXT_REPZ_SCAS_H_
#define __MCFCRT_EXT_REPZ_SCAS_H_

#include "../env/_crtdef.h"

#ifndef __MCFCRT_REPZ_SCAS_INLINE_OR_EXTERN
#  define __MCFCRT_REPZ_SCAS_INLINE_OR_EXTERN     __attribute__((__gnu_inline__)) extern inline
#endif

_MCFCRT_EXTERN_C_BEGIN

__MCFCRT_REPZ_SCAS_INLINE_OR_EXTERN int _MCFCRT_repz_scasb(void **_MCFCRT_RESTRICT __es, const void *__s, int __c, _MCFCRT_STD size_t __n) _MCFCRT_NOEXCEPT {
	bool __z, __b;
	const _MCFCRT_STD uint8_t *__p = (const _MCFCRT_STD uint8_t *)__s;
	__asm__ (
		"repz scasb \n"
		: "+D"(__p), "=@ccz"(__z), "=@ccb"(__b)
		: "m"(*__p), "a"(__c), "c"(__n)
	);
	if(__es){
		*__es = (_MCFCRT_STD uint8_t *)(__p + __z - 1);
	}
	return __z ? 0 : (__b ? -1 : 1);
}
__MCFCRT_REPZ_SCAS_INLINE_OR_EXTERN int _MCFCRT_repz_scasw(_MCFCRT_STD uint16_t **_MCFCRT_RESTRICT __es, const _MCFCRT_STD uint16_t *__s, int __c, _MCFCRT_STD size_t __n) _MCFCRT_NOEXCEPT {
	bool __z, __b;
	const _MCFCRT_STD uint16_t *__p = __s;
	__asm__ (
		"repz scasw \n"
		: "+D"(__p), "=@ccz"(__z), "=@ccb"(__b)
		: "m"(*__p), "a"(__c), "c"(__n)
	);
	if(__es){
		*__es = (_MCFCRT_STD uint16_t *)(__p + __z - 1);
	}
	return __z ? 0 : (__b ? -1 : 1);
}
__MCFCRT_REPZ_SCAS_INLINE_OR_EXTERN int _MCFCRT_repz_scasd(_MCFCRT_STD uint32_t **_MCFCRT_RESTRICT __es, const _MCFCRT_STD uint32_t *__s, int __c, _MCFCRT_STD size_t __n) _MCFCRT_NOEXCEPT {
	bool __z, __b;
	const _MCFCRT_STD uint32_t *__p = __s;
	__asm__ (
		"repz scasd \n"
		: "+D"(__p), "=@ccz"(__z), "=@ccb"(__b)
		: "m"(*__p), "a"(__c), "c"(__n)
	);
	if(__es){
		*__es = (_MCFCRT_STD uint32_t *)(__p + __z - 1);
	}
	return __z ? 0 : (__b ? -1 : 1);
}
#ifdef _WIN64
__MCFCRT_REPZ_SCAS_INLINE_OR_EXTERN int _MCFCRT_repz_scasq(_MCFCRT_STD uint64_t **_MCFCRT_RESTRICT __es, const _MCFCRT_STD uint64_t *__s, int __c, _MCFCRT_STD size_t __n) _MCFCRT_NOEXCEPT {
	bool __z, __b;
	const _MCFCRT_STD uint64_t *__p = __s;
	__asm__ (
		"repz scasq \n"
		: "+D"(__p), "=@ccz"(__z), "=@ccb"(__b)
		: "m"(*__p), "a"(__c), "c"(__n)
	);
	if(__es){
		*__es = (_MCFCRT_STD uint64_t *)(__p + __z - 1);
	}
	return __z ? 0 : (__b ? -1 : 1);
}
#endif

_MCFCRT_EXTERN_C_END

#endif
