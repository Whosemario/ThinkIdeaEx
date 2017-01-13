cmake_minimum_required(VERSION 2.6)

add_executable(function_args_test function_args_test.cpp)
if(WIN32)
else()
set_target_properties(rpc PROPERTIES COMPILE_FLAGS "-std=c++14")
endif()
target_link_libraries(function_args_test glog)

