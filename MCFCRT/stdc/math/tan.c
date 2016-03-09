// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include "_math_asm.h"
#include "_constants.h"

float tanf(float x){
	register float ret;
	__asm__ __volatile__(
		"fld dword ptr[%1] \n"
		"fptan \n"
		"fstsw ax \n"
		"test ah, 4 \n"
		"jz 1f \n"
		"	fld tbyte ptr[%2] \n"
		"	fxch st(1) \n"
		"	2: \n"
		"		fprem1 \n"
		"		fstsw ax \n"
		"		test ah, 4 \n"
		"	jnz 2b \n"
		"	fstp st(1) \n"
		"	fptan \n"
		"1: \n"
		"fstp st \n"
		__FLT_RET_ST("%1")
		: __FLT_RET_CONS(ret)
		: "m"(x), "m"(__MCFCRT_kMath_Pos_1p61_Pi)
		: "ax"
	);
	return ret;
}

double tan(double x){
	register double ret;
	__asm__ __volatile__(
		"fld qword ptr[%1] \n"
		"fptan \n"
		"fstsw ax \n"
		"test ah, 4 \n"
		"jz 1f \n"
		"	fld tbyte ptr[%2] \n"
		"	fxch st(1) \n"
		"	2: \n"
		"		fprem1 \n"
		"		fstsw ax \n"
		"		test ah, 4 \n"
		"	jnz 2b \n"
		"	fstp st(1) \n"
		"	fptan \n"
		"1: \n"
		"fstp st \n"
		__DBL_RET_ST("%1")
		: __DBL_RET_CONS(ret)
		: "m"(x), "m"(__MCFCRT_kMath_Pos_1p61_Pi)
		: "ax"
	);
	return ret;
}

long double tanl(long double x){
	register long double ret;
	__asm__ __volatile__(
		"fld tbyte ptr[%1] \n"
		"fptan \n"
		"fstsw ax \n"
		"test ah, 4 \n"
		"jz 1f \n"
		"	fld tbyte ptr[%2] \n"
		"	fxch st(1) \n"
		"	2: \n"
		"		fprem1 \n"
		"		fstsw ax \n"
		"		test ah, 4 \n"
		"	jnz 2b \n"
		"	fstp st(1) \n"
		"	fptan \n"
		"1: \n"
		"fstp st \n"
		__LDBL_RET_ST("%1")
		: __LDBL_RET_CONS(ret)
		: "m"(x), "m"(__MCFCRT_kMath_Pos_1p61_Pi)
		: "ax"
	);
	return ret;
}
