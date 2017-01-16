/*
 */

#include "rpc/rpc_service.hpp"

namespace hex_engine {
namespace rpc {

std::size_t rpc_request::parse(const char* data, std::size_t sz) {
	std::size_t orig = sz;
	while (sz) {
		if (status_ == ST_HEAD) {
			if (sz >= cosume_size_) {
				stream_ptr_->write_stream().write(data, cosume_size_);
				sz -= cosume_size_;
				status_ = ST_BODY;
				data += cosume_size_;
				cosume_size_ = total_size();
			}
			else {
				stream_ptr_->write_stream().write(data, sz);
				cosume_size_ -= sz;
				sz = 0;
				break;
			}
		}
		else if (status_ == ST_BODY) {
			if (sz >= cosume_size_) {
				stream_ptr_->write_stream().write(data, cosume_size_);
				sz -= cosume_size_;
				status_ = ST_END;
				break;
			}
			else {
				stream_ptr_->write_stream().write(data, sz);
				cosume_size_ -= sz;
				sz = 0;
				break;
			}
		}
		else {
			break;
		}
	}
	return orig - sz;
}
}
}