// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_EXT_REP_STOS_H_
#define __MCFCRT_EXT_REP_STOS_H_

#include "../env/_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

#define __MCFCRT_REP_STOS_DEFINE(__name_, __mnemonic_, __type_)	\
	__type_ *__name_(__type_ *_MCFCRT_RESTRICT __s, __type_ __c, _MCFCRT_STD size_t __n) _MCFCRT_NOEXCEPT {	\
		__type_ *__p = __s;	\
		_MCFCRT_STD size_t __unused;	\
		__asm__ (	\
			__mnemonic_ " \n"	\
			: "=o"(*(char (*)[])__p), "+D"(__p), "=c"(__unused)	\
			: "a"(__c), "c"(__n)	\
		);	\
		return __p;	\
	}

static inline __MCFCRT_REP_STOS_DEFINE(_MCFCRT_rep_stosb, "rep stosb", _MCFCRT_STD  uint8_t)
static inline __MCFCRT_REP_STOS_DEFINE(_MCFCRT_rep_stosw, "rep stosw", _MCFCRT_STD uint16_t)
static inline __MCFCRT_REP_STOS_DEFINE(_MCFCRT_rep_stosd, "rep stosd", _MCFCRT_STD uint32_t)
#ifdef _WIN64
static inline __MCFCRT_REP_STOS_DEFINE(_MCFCRT_rep_stosq, "rep stosq", _MCFCRT_STD uint64_t)
#endif

_MCFCRT_EXTERN_C_END

#endif
