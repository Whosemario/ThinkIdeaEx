cmake_minimum_required(VERSION 2.6)

set(SRCS
	asio.hpp
	
	deadline_timer.hpp
	io_service.hpp
	io_context.hpp
	io_service_strand.hpp
	io_context_strand.hpp
	
	detail/config.hpp
	impl/src.hpp
	asio.cpp
	)

add_library(asio STATIC ${SRCS})
if(WIN32)
else()
set_target_properties(asio PROPERTIES COMPILE_FLAGS "-std=c++14")
endif()
