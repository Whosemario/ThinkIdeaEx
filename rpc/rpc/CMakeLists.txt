cmake_minimum_required(VERSION 2.6)

set(SRCS
	function_traits.hpp
	rpc_stream.hpp
	rpc_stream.cpp
	rpc_service.hpp
	rpc_service.cpp
	rpc_request.hpp
	rpc_request.cpp
)

add_library(rpc STATIC ${SRCS})
if(WIN32)
elseif(APPLE)
set_target_properties(rpc PROPERTIES COMPILE_FLAGS "-std=c++1y")
else()
set_target_properties(rpc PROPERTIES COMPILE_FLAGS "-std=c++14")
endif()
target_link_libraries(rpc glog asio)
