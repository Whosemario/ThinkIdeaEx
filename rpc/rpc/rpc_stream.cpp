
#include "rpc/rpc_stream.hpp"

namespace hex_engine {
namespace rpc {

rpc_stream::rpc_stream() : stream_(&buf_)
{}

void rpc_stream::reset() {
    write_stream().seekp(0, std::ios_base::beg);
    read_stream().seekg(0, std::ios_base::beg);
}

char* rpc_stream::c_str() {
	return (char*)const_cast<void*>(asio::buffer_cast<const void*>(buf().data())); 
}

}
}