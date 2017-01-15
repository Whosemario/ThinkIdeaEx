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
    
    std::size_t parse(const char* data, std::size_t sz) {
        std::size_t orig = sz;
        while(sz) {
            if(status_ == ST_HEAD) {
                if(sz >= resume_size_) {
                    stream_ptr_->write_stream().write(data, resume_size_);
                    sz -= resume_size_;
                    status_ = ST_BODY;
                    data += resume_size_;
                    resume_size_ = total_size();
                } else {
                    stream_ptr_->write_stream().write(data, sz);
                    resume_size_ -= sz;
                    sz = 0;
                    break;
                }
            } else if(status_ == ST_BODY) {
                if(sz >= resume_size_) {
                    
                }
            } else {
                break;
            }
        }
        return orig - sz;
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