/*
 */

#pragma once

#include "rpc/rpc_stream.hpp"

#include <asio/detail/noncopyable.hpp>
#include <string>
#include <tuple>

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
        status_(ST_HEAD), cosume_size_(kTHeadSize),
        total_size_(0), stream_ptr_(new rpc_stream()) {}
    
    uint32_t total_size() {
        if(total_size_) return total_size_;
        stream_ptr_->read_stream().seekg(0, std::ios_base::beg);
        stream_ptr_->read_stream().read((char*)(&total_size_), kTHeadSize);
        return total_size_;
    }
    
	std::size_t parse(const char* data, std::size_t sz);

	void reset() {
		stream_ptr_->reset();
		status_ = ST_HEAD;
		cosume_size_ = kTHeadSize;
		total_size_ = 0;
	}
    
    bool is_completed() const { return status_ == ST_END; }
    
    std::tuple<std::string, std::string> method_and_args() {
		stream_ptr_->read_stream().seekg(kTHeadSize, std::ios_base::beg);
		uint8_t method_sz = 0;
		stream_ptr_->read_stream().read((char*)(&method_sz), kMHeadSize);
		assert(method_sz);
		char* p = stream_ptr_->c_str();
		return std::make_tuple(
					std::string(p, method_sz), 
					std::string(p+method_sz, total_size() - kMHeadSize - method_sz));
    }

    
private:
    const static std::size_t kTHeadSize = 4;
    const static std::size_t kMHeadSize = 1;
    request_status status_;
    std::size_t cosume_size_;
    uint32_t total_size_;
    rpc_stream_ptr stream_ptr_;
};
    
}
}