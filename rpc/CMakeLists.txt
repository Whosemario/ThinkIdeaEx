cmake_minimum_required(VERSION 2.8)


project(rpc)

if (CMAKE_BUILD_TYPE)
else()
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

message("--------- Configure ---------")
message("   CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if (WIN32) 
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
else ()
endif ()

message("   CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("   CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

# header dir
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/msgpack)

# for asio
add_definitions(
	-DASIO_STANDALONE 
	-DASIO_SEPARATE_COMPILATION 
	#-DASIO_HAS_BOOST_BIND
	#-DASIO_HAS_BOOST_DATE_TIME
)

# sub module
add_subdirectory(glog)
add_subdirectory(asio)
add_subdirectory(rpc)
add_subdirectory(tests)

message("-----------------------------")