// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#ifndef MCF_HASH_SHA1_HPP_
#define MCF_HASH_SHA1_HPP_

#include <cstddef>
#include <cstdint>

namespace MCF {

class Sha1 {
private:
	bool xm_bInited;
	std::uint32_t xm_auResult[5];

	union {
		unsigned char aby[64];
		struct {
			unsigned char abyData[56];
			std::uint64_t u64Bits;
		} vLast;
	} xm_vChunk;

	std::size_t xm_uBytesInChunk;
	std::uint64_t xm_u64BytesTotal;

public:
	Sha1() noexcept;

public:
	void Abort() noexcept;
	void Update(const void *pData, std::size_t uSize) noexcept;
	void Finalize(unsigned char (&abyOutput)[20]) noexcept;
};

}

#endif
