// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef MCF_STREAMS_ABSTRACT_INPUT_STREAM_FILTER_HPP_
#define MCF_STREAMS_ABSTRACT_INPUT_STREAM_FILTER_HPP_

#include "AbstractInputStream.hpp"

namespace MCF {

class AbstractInputStreamFilter : public AbstractInputStream {
protected:
	IntrusivePtr<AbstractInputStream> x_pUnderlyingStream;

public:
	explicit AbstractInputStreamFilter(IntrusivePtr<AbstractInputStream> pUnderlyingStream) noexcept
		: x_pUnderlyingStream(std::move(pUnderlyingStream))
	{
	}
	virtual ~AbstractInputStreamFilter() = 0;

	AbstractInputStreamFilter(AbstractInputStreamFilter &&) noexcept = default;
	AbstractInputStreamFilter& operator=(AbstractInputStreamFilter &&) noexcept = default;

public:
	virtual int Peek() const = 0;
	virtual int Get() = 0;
	virtual bool Discard() = 0;

	virtual std::size_t Peek(void *pData, std::size_t uSize) const = 0;
	virtual std::size_t Get(void *pData, std::size_t uSize) = 0;
	virtual std::size_t Discard(std::size_t uSize) = 0;

	const IntrusivePtr<AbstractInputStream> &GetUnderlyingStream() const noexcept {
		return x_pUnderlyingStream;
	}
	IntrusivePtr<AbstractInputStream> &GetUnderlyingStream() noexcept {
		return x_pUnderlyingStream;
	}
	void SetUnderlyingStream(IntrusivePtr<AbstractInputStream> pUnderlyingStream) noexcept {
		x_pUnderlyingStream = std::move(pUnderlyingStream);
	}
};

}

#endif
