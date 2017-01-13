cmake_minimum_required(VERSION 2.6)

add_executable(function_args_test function_args_test.cpp)
if(WIN32)
elseif(APPLE)
set_target_properties(function_args_test PROPERTIES COMPILE_FLAGS "-std=c++1y")
else()
set_target_properties(function_args_test PROPERTIES COMPILE_FLAGS "-std=c++14")
endif()
target_link_libraries(function_args_test glog)

