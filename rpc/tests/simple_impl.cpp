/*
 */


#include "rpc/rpc_stream.hpp"

#include <msgpack.hpp>

#include <utility>
#include <tuple>
#include <iostream>
#include <string>


class rpc_service {
public:
    rpc_service(): stream_ptr_(new hex_engine::rpc::rpc_stream()) {}
    template<typename ... Args>
    void remote_call(const std::string& method, Args&& ... args) {
        auto t = std::make_tuple(std::forward<Args>(args)...);
        uint32_t total_sz = 1;
        stream_ptr_->write_stream().write((char*)&total_sz, sizeof(total_sz));
        uint8_t method_sz = method.size();
        stream_ptr_->write_stream().write((char*)&method_sz, sizeof(method_sz));
        stream_ptr_->write_stream().write(method.c_str(), method_sz);
        msgpack::pack(stream_ptr_->write_stream(), t);
        total_sz = stream_ptr_->buf().size() - sizeof(total_sz);
        stream_ptr_->write_stream().seekp(0, std::ios_base::beg);
        stream_ptr_->write_stream().write((char*)&total_sz, sizeof(total_sz));
    }
    
    hex_engine::rpc::rpc_stream_ptr stream_ptr() { return stream_ptr_; }
    
private:
    hex_engine::rpc::rpc_stream_ptr stream_ptr_;
};


int main(int args, char* argv[])
{
    rpc_service service;
    service.remote_call("add", 100, 3);
    
    hex_engine::rpc::rpc_stream_ptr ptr = service.stream_ptr();
    
    uint32_t total_sz = 0;
    ptr->read_stream().read((char*)(&total_sz), sizeof(total_sz));
    
    std::cout << total_sz << std::endl;
    
    
    /*
    //std::cout << service.stream_ptr()->buf().size() << std::endl;
    
    std::string str(service.stream_ptr()->c_str(), service.stream_ptr()->buf().size());
    //std::cout << str.length() << " " << str.size() << std::endl;
    
    std::tuple<int, int> dest;
    msgpack::unpack(
        str.c_str(),
        str.size()
    ).get().convert(dest);
    
    std::cout << std::get<0>(dest) << " " << std::get<1>(dest) << std::endl;
    */
    return 0;
}
