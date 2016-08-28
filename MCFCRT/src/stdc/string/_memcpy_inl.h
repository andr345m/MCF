// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_STRING_MEMCPY_INL_H_
#define __MCFCRT_STRING_MEMCPY_INL_H_

#include "../../env/_crtdef.h"
#include <emmintrin.h>

_MCFCRT_EXTERN_C_BEGIN

__attribute__((__always_inline__))
static inline void __MCFCRT_CopyForward(void *__s1, const void *__s2, _MCFCRT_STD size_t __bytes) _MCFCRT_NOEXCEPT {
	register char *__wp = __s1;
	char *const __wend = __wp + __bytes;
	register const char *__rp = __s2;
	while(((_MCFCRT_STD uintptr_t)__wp & 15) != 0){
		if(__wp == __wend){
			return;
		}
		*(__wp++) = *(__rp++);
	}
	_MCFCRT_STD size_t __t;
	if((__t = (_MCFCRT_STD size_t)(__wend - __wp) / 16) != 0){
		char *const __xmmwend = __wp + __t * 16;
#define __MCFCRT_SSE2_STEP(__si_, __li_)	\
		{	\
			(__si_)((__m128i *)__wp, (__li_)((const __m128i *)__rp));	\
			__wp += 16;	\
			__rp += 16;	\
		}
#define __MCFCRT_SSE2_FULL(__si_, __li_)	\
		{	\
			switch(__t % 8){	\
				do { _mm_prefetch(__rp + 256, _MM_HINT_NTA);	\
			default: __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 7:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 6:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 5:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 4:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 3:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 2:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 1:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
				} while(__wp != __xmmwend);	\
			}	\
		}
		if(__t < 0x1000){
			if(((_MCFCRT_STD uintptr_t)__rp & 15) != 0){
				__MCFCRT_SSE2_FULL(_mm_store_si128, _mm_loadu_si128)
			} else {
				__MCFCRT_SSE2_FULL(_mm_store_si128, _mm_load_si128)
			}
		} else {
			if(((_MCFCRT_STD uintptr_t)__rp & 15) != 0){
				__MCFCRT_SSE2_FULL(_mm_stream_si128, _mm_loadu_si128)
			} else {
				__MCFCRT_SSE2_FULL(_mm_stream_si128, _mm_load_si128)
			}
		}
#undef __MCFCRT_SSE2_STEP
#undef __MCFCRT_SSE2_FULL
	}
	if((__t = (_MCFCRT_STD size_t)(__wend - __wp)) != 0){
		_MCFCRT_STD uintptr_t __z;
		__asm__ volatile (
#ifdef _WIN64
			"shr rcx, 3 \n"
			"rep movsq \n"
			"mov rcx, rax \n"
			"and rcx, 7 \n"
#else
			"shr ecx, 2 \n"
			"rep movsd \n"
			"mov ecx, eax \n"
			"and ecx, 3 \n"
#endif
			"rep movsb \n"
			: "=D"(__z), "=S"(__z), "=c"(__z), "=a"(__z)
			: "D"(__wp), "S"(__rp), "c"(__t), "a"(__t)
		);
	}
}

__attribute__((__always_inline__))
static inline void __MCFCRT_CopyBackward(void *__s1, const void *__s2, _MCFCRT_STD size_t __bytes) _MCFCRT_NOEXCEPT {
	char *const __wbegin = __s1;
	register char *__wp = __wbegin + __bytes;
	register const char *__rp = (const char *)__s2 + __bytes;
	while(((_MCFCRT_STD uintptr_t)__wp & 15) != 0){
		if(__wbegin == __wp){
			return;
		}
		*(--__wp) = *(--__rp);
	}
	_MCFCRT_STD size_t __t;
	if((__t = (_MCFCRT_STD size_t)(__wp - __wbegin) / 16) != 0){
		char *const __xmmbegin = __wp - __t * 16;
#define __MCFCRT_SSE2_STEP(__si_, __li_)	\
		{	\
			__wp -= 16;	\
			__rp -= 16;	\
			(__si_)((__m128i *)__wp, (__li_)((const __m128i *)__rp));	\
		}
#define __MCFCRT_SSE2_FULL(__si_, __li_)	\
		{	\
			switch(__t % 8){	\
				do { _mm_prefetch(__rp - 256, _MM_HINT_NTA);	\
			default: __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 7:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 6:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 5:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 4:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 3:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 2:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
			case 1:  __MCFCRT_SSE2_STEP(__si_, __li_)	\
				} while(__xmmbegin != __wp);	\
			}	\
		}
		if(__t < 0x1000){
			if(((_MCFCRT_STD uintptr_t)__rp & 15) != 0){
				__MCFCRT_SSE2_FULL(_mm_store_si128, _mm_loadu_si128)
			} else {
				__MCFCRT_SSE2_FULL(_mm_store_si128, _mm_load_si128)
			}
		} else {
			if(((_MCFCRT_STD uintptr_t)__rp & 15) != 0){
				__MCFCRT_SSE2_FULL(_mm_stream_si128, _mm_loadu_si128)
			} else {
				__MCFCRT_SSE2_FULL(_mm_stream_si128, _mm_load_si128)
			}
		}
#undef __MCFCRT_SSE2_STEP
#undef __MCFCRT_SSE2_FULL
	}
	if((__t = (_MCFCRT_STD size_t)(__wp - __wbegin)) != 0){
		_MCFCRT_STD uintptr_t __z;
		__asm__ volatile (
			"std \n"
#ifdef _WIN64
			"sub rdi, 8 \n"
			"sub rsi, 8 \n"
			"shr rcx, 3 \n"
			"rep movsq \n"
			"add rdi, 7 \n"
			"add rsi, 7 \n"
			"mov rcx, rax \n"
			"and rcx, 7 \n"
#else
			"sub edi, 4 \n"
			"sub esi, 4 \n"
			"shr ecx, 2 \n"
			"rep movsd \n"
			"add edi, 3 \n"
			"add esi, 3 \n"
			"mov ecx, eax \n"
			"and ecx, 3 \n"
#endif
			"rep movsb \n"
			"cld \n"
			: "=D"(__z), "=S"(__z), "=c"(__z), "=a"(__z)
			: "D"(__wp), "S"(__rp), "c"(__t), "a"(__t)
		);
	}
}

_MCFCRT_EXTERN_C_END

#endif
