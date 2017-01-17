/*
 */

#pragma once

#include "rpc/rpc_stream.hpp"
#include "rpc/rpc_request.hpp"
#include "rpc/function_traits.hpp"

#include <asio/detail/noncopyable.hpp>
#include <msgpack.hpp>

#include <memory>
#include <string>
#include <tuple>


namespace hex_engine {
namespace rpc {

// handler trait
template<typename Cls>
struct handler_base {
	virtual void call(Cls*, const std::string&) = 0;
};

template<typename Cls, typename Handler>
struct handler_impl : public handler_base<Cls> {

	handler_impl(Handler& handler) :
		handler_(handler) {}

	virtual void call(Cls* self, const std::string& args_str) {
		using args_tuple = typename function_traits<Handler>::tuple_type;
		args_tuple t;
		msgpack::unpack(args_str.c_str(), args_str.size()).get().convert(t);
        using index = std::make_index_sequence<std::tuple_size<std::remove_reference_t<args_tuple>>::value>;
        call_impl(self, t, index{});
	}

	template<typename TupleArgs, std::size_t ... Is>
	void call_impl(Cls* self, TupleArgs&& tuple_args, std::index_sequence<Is...>) {
		(self->*handler_)(std::forward<typename std::tuple_element<Is, std::remove_reference_t<TupleArgs>>::type>(std::get<Is>(tuple_args))...);
	}

	Handler handler_;
};
    
class rpc_service : public asio::noncopyable {
public:
    void handle_data(const char*, std::size_t);
	virtual void call_method(const std::string&, const std::string&) = 0;
private:
	rpc_request request_;
};

typedef std::shared_ptr<rpc_service> rpc_service_ptr;

}
}