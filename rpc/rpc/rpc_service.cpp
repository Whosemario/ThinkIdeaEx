/*
 */

#include "rpc/rpc_service.hpp"

namespace hex_engine {
namespace rpc {
    
void rpc_service::handle_data(const char* data, std::size_t sz) {
    while(sz) {
        request_.reset();
        std::size_t consume_sz = request_.parse(data, sz);
        data += consume_sz;
        sz -= consume_sz;
        if(request_.is_completed()) {
            
        }
    }
}
    
}
}