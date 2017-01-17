/*
 */

#include "rpc/rpc_service.hpp"

#include <unordered_map>
#include <string>
#include <memory>

namespace hex = hex_engine::rpc;

template<typename Derive>
class logic_service : public hex::rpc_service {
public:
	typedef Derive subclass;
	typedef hex::handler_base<subclass> handler_cls;
	typedef std::shared_ptr<handler_cls> handler_cls_ptr;
	typedef std::unordered_map<std::string, handler_cls_ptr> func_map; 

public:
	template<typename Handler>
	void register_func(const std::string& method, Handler&& handler) {
		handler_cls_ptr ptr = std::make_shared<hex::handler_impl<subclass, Handler>>(handler);
		register_.emplace(method, ptr);
	}

	virtual void call_method(const std::string& method, const std::string& args) {
		auto it = register_.find(method);
		if (it != register_.end()) {
			it->second->call(dynamic_cast<subclass*>(this), args);
		}
	}

private:
	func_map register_;
};

class client_gate_service : public logic_service<client_gate_service> {
public:
	client_gate_service() {
		register_func("connect_server", &client_gate_service::connect_server);
	}

	void connect_server(const std::string& id) {
		std::cout << "id is " << id << std::endl;
	}
};

template<typename ... Args>
void remote_call(hex::rpc_stream_ptr stream_ptr_, const std::string& method, Args&& ... args) {
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
	stream_ptr_->write_stream().seekp(total_sz + 4, std::ios_base::beg);
}

int main(int args, char* argv[])
{
	client_gate_service service;
	
	hex::rpc_stream_ptr stream = std::make_shared<hex::rpc_stream>();
	remote_call(stream, "connect_server", "hello");
	std::string str(stream->c_str(), stream->buf().size());
	/*
	uint32_t t = 0;
	stream->read_stream().read((char*)(&t), 4);
	std::cout << t << std::endl;
	*/
	service.handle_data(str.c_str(), str.size());
	return 0;
}