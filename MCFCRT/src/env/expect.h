// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2017, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_EXPECT_H_
#define __MCFCRT_ENV_EXPECT_H_

#include "_crtdef.h"

#define _MCFCRT_EXPECT(__x_)        (__builtin_expect((__x_) ? true : false, true ))
#define _MCFCRT_EXPECT_NOT(__x_)    (__builtin_expect((__x_) ? true : false, false))

#endif
