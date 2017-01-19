cmake_minimum_required(VERSION 2.6)

set(SRCS
	common.hpp
	proto.cpp
)

add_library(proto STATIC ${SRCS})
if(WIN32)
elseif(APPLE)
set_target_properties(proto PROPERTIES COMPILE_FLAGS "-std=c++1y")
else()
set_target_properties(proto PROPERTIES COMPILE_FLAGS "-std=c++14")
endif()
