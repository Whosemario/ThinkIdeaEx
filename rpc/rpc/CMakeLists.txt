cmake_minimum_required(VERSION 2.6)

add_definitions(-DASIO_DISABLE_BOOST_BIND)

set(SRCS
	rpc_stream.hpp
	rpc_stream.cpp
	)

add_library(rpc STATIC ${SRCS})
if(WIN32)
else()
set_target_properties(rpc PROPERTIES COMPILE_FLAGS "-std=c++14")
endif()
target_link_libraries(rpc glog)
