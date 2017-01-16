/*
 */

#include "rpc/rpc_stream.hpp"
#include "rpc/rpc_request.hpp"
#include <asio/detail/noncopyable.hpp>

#include <memory>


namespace hex_engine {
namespace rpc {
    
class rpc_service : public asio::noncopyable {
public:
    void handle_data(const char*, std::size_t);
private:
	rpc_request request_;
};

typedef std::shared_ptr<rpc_service> rpc_service_ptr;

}
}