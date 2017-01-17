/*
 */

#pragma once

#include <utility>
#include <type_traits>
#include <tuple>

namespace hex_engine {
namespace rpc {

template<typename T>
struct function_traits_impl;

template<typename T>
struct function_traits :
	function_traits_impl<std::remove_cv_t<std::remove_reference_t<T>>>
{};

template<typename Ret, typename ... Args>
struct function_traits_impl<Ret(Args...)> {
	typedef Ret return_type;
	
	template<std::size_t I>
	struct args {
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};

	typedef std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...> tuple_type;
};

template<typename Ret, typename ... Args>
struct function_traits_impl<Ret(*)(Args...)>: 
	function_traits_impl<Ret(Args...)> {};

template<typename Ret, typename Cls, typename ... Args>
struct function_traits_impl<Ret(Cls::*)(Args...)>: 
	function_traits_impl<Ret(Args...)> {};

}
}