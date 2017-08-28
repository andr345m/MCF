// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include "../../env/expect.h"

#undef memcmp

static inline uintptr_t bswap_ptr(uintptr_t w){
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return _Generic(w,
		uint32_t: __builtin_bswap32((uint32_t)w),
		uint64_t: __builtin_bswap64((uint64_t)w));
#else
	return w;
#endif
}

int memcmp(const void *s1, const void *s2, size_t n){
	const unsigned char *rp1 = s1;
	const unsigned char *rp2 = s2;
	size_t rem = n / sizeof(uintptr_t);
	if(_MCFCRT_EXPECT_NOT(rem != 0)){
		switch((rem - 1) % 32){
			uintptr_t w, c;
		diff_wc:
			w = bswap_ptr(w);
			c = bswap_ptr(c);
			return (w < c) ? -1 : 1;
#define STEP(k_)	\
				__attribute__((__fallthrough__));	\
		case (k_):	\
				__builtin_memcpy(&w, rp1, sizeof(w));	\
				__builtin_memcpy(&c, rp2, sizeof(c));	\
				--rem;	\
				if(_MCFCRT_EXPECT_NOT(w != c)){	\
					goto diff_wc;	\
				}	\
				rp1 += sizeof(w);	\
				rp2 += sizeof(c);
//=============================================================================
			do {
		STEP(037)  STEP(036)  STEP(035)  STEP(034)  STEP(033)  STEP(032)  STEP(031)  STEP(030)
		STEP(027)  STEP(026)  STEP(025)  STEP(024)  STEP(023)  STEP(022)  STEP(021)  STEP(020)
		STEP(017)  STEP(016)  STEP(015)  STEP(014)  STEP(013)  STEP(012)  STEP(011)  STEP(010)
		STEP(007)  STEP(006)  STEP(005)  STEP(004)  STEP(003)  STEP(002)  STEP(001)  STEP(000)
			} while(_MCFCRT_EXPECT(rem != 0));
//=============================================================================
#undef STEP
		}
	}
	rem = n % sizeof(uintptr_t);
	while(_MCFCRT_EXPECT(rem != 0)){
		if(*rp1 != *rp2){
			return (*rp1 < *rp2) ? -1 : 1;
		}
		++rp1;
		++rp2;
		--rem;
	}
	return 0;
}
