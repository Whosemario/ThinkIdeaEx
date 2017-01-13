/*
 */

#include <tuple>	// std::tuple
#include <utility>	// std::forward, std::get
#include <iostream>
#include <string>
#include <sstream>	// stringstream
#include <type_traits>	// std::is_void

#include <msgpack.hpp>


template<typename ... Args>
void func(const std::string& str, Args&& ... args) {
	auto tuple = std::make_tuple(str, std::forward<Args>(args)...);
	std::cout << std::get<2>(tuple) << std::endl;
}

///////////////////////////

template<typename T>
struct function_traits_impl;

template<typename T>
struct function_traits :
	function_traits_impl<std::remove_cv_t<std::remove_reference_t<T>>>
{};

template<typename Ret, typename ... Args>
struct function_traits_impl<Ret(Args...)> {
public:
	typedef Ret return_type;

	template<std::size_t I>
	struct args {
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};

	typedef std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...> tuple_type;
};

template<typename Ret, typename ... Args>
struct function_traits_impl<Ret(*)(Args...)> : function_traits_impl<Ret(Args...)> {
};

template<typename Handler>
void process(Handler&& handler) {
	std::cout << std::is_void<typename function_traits<Handler>::return_type>::value << std::endl;
	std::cout << std::is_same<int, typename function_traits<Handler>::args<0>::type>::value << std::endl;
}

int func(float a) { return 1; }

void func2(int a, bool b, const std::string& s) {
	std::cout << a << b<< s << std::endl;
}

template<typename Handler, typename TupleArgs, std::size_t ... Is>
void call_func_impl(Handler&& handler, TupleArgs&& tuple_args, std::index_sequence<Is...>) {
	handler(std::forward<std::tuple_element_t<Is, std::remove_reference_t<TupleArgs>>>(std::get<Is>(tuple_args))...);
}

template<typename Handler>
void process2(Handler&& handler, const std::string& str) {
	using args_tuple = typename function_traits<Handler>::tuple_type;
	args_tuple t;
	msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
	oh.get().convert(t);

	using index = std::make_index_sequence<std::tuple_size<std::remove_reference_t<args_tuple>>::value>;
	call_func_impl(
		std::forward<Handler>(handler),
		t,
		index{});
}

int main(int args, char* argv[]) {
	std::tuple<int, bool, std::string> t(1, false, "hi");
	std::stringstream buffer;
	msgpack::pack(buffer, t);

	buffer.seekg(0);
	std::string s(buffer.str());

	process2(func2, s);

	return 1;
}