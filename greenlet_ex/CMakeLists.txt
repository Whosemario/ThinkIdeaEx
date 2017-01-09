cmake_minimum_required(VERSION 2.8)


project(greenlet)

if (CMAKE_BUILD_TYPE)
else()
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

message("--------- Configure ---------")
message("   CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("   PYTHON_DIR: ${PYTHON_DIR}")

if (WIN32) 
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	include_directories(${PYTHON_DIR}/include)
	link_directories(${PYTHON_DIR}/libs)
	set(PYTHON_LIB python27 CACHE INTERNAL "python lib")
else ()
	include_directories(${PYTHON_DIR}/include/python2.7)
	link_directories(${PYTHON_DIR}/lib)
	set(PYTHON_LIB python2.7 CACHE INTERNAL "python lib")
endif ()

message("   CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("   CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

add_subdirectory(third_lib/glog)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(GREENLET_SRC
	src/slp_platformselect.h
	src/greenlet.h
	src/greenlet.c
)
if(WIN32)
	list(APPEND GREENLET_SRC "${PROJECT_SOURCE_DIR}/src/platform/switch_x64_masm.obj")
endif()
message("   GREENLET_SRC: ${GREENLET_SRC}")

add_library(greenlet MODULE ${GREENLET_SRC})
if(WIN32)
	set_target_properties(greenlet PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(greenlet ${PYTHON_LIB})

message("-----------------------------")