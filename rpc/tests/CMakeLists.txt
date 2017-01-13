cmake_minimum_required(VERSION 2.6)

add_executable(function_args_test function_args_test.cpp)
set_target_properties(function_args_test PROPERTIES COMPILE_FLAGS "-std=c++11")
#target_link_libraries(function_args_test glog)

