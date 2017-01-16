/*
 */

#include "rpc/rpc_stream.hpp"

#include <asio/detail/noncopyable.hpp>
#include <string>

namespace hex_engine {
namespace rpc {
    
enum request_status {
    ST_HEAD = 0,
    ST_BODY = 1,
    ST_END = 2
};

class rpc_request : public asio::noncopyable {
public:
    rpc_request():
        status_(ST_HEAD), resume_size_(kTHeadSize),
        total_size_(0), stream_ptr_(new rpc_stream()) {}
    
    std::size_t total_size() {
        if(total_size_) return total_size_;
        stream_ptr_->read_stream().seekg(0, std::ios_base::beg);
        stream_ptr_->read_stream().read((char*)total_size_, kTHeadSize);
        return total_size_;
    }
    
	std::size_t parse(const char* data, std::size_t sz);

	void reset() {
		stream_ptr_->reset();
		status_ = ST_HEAD;
		resume_size_ = kTHeadSize;
		total_size_ = 0;
	}
    
private:
    const static std::size_t kTHeadSize = 4;
    const static std::size_t kMHeadSize = 1;
    request_status status_;
    std::size_t resume_size_;
    std::size_t total_size_;
    rpc_stream_ptr stream_ptr_;
};
    
}
}