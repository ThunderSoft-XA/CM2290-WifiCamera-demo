#Cmake lowest  version 
CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
 
#project name
PROJECT(gst_test CXX)
 
#allow gdb debug
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
 
#add C++11 support and other options
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wno-deprecated")
#set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS -g -ftest-coverage -fprofile-arcs"})

find_package(PkgConfig REQUIRED)
pkg_check_modules(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 glib-2.0 gobject-2.0 gstreamer-rtsp-server-1.0)
 
# include file dir
include_directories(
    ${PROJECT_SOURCE_DIR}
    /usr/include/gstreamer-1.0/
    /usr/include/glib-2.0/
)
 
#add sub dir
#add_subdirectory(dict) 
 
#set all sources files in current dir to DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

message(INFO "source file: " ${DIR_SRCS})
 
#get a executabl file
add_executable(gst_test 
    ./gst-test.cpp) 

#get a executabl file
add_executable(gst_test_base 
    ./gst-test-base.cpp) 

#get a executabl file
add_executable(gst_test_server 
    ./gst-test-server.cpp) 
 
#add link libs
target_link_libraries(gst_test PRIVATE PkgConfig::gstreamer
    ${GST_LIBRARIES}
    ${GST_RTSP_LIBRARIES}
    pthread
    gstreamer-1.0
    gobject-2.0
    glib-2.0
    m
    z
)

#add link libs
target_link_libraries(gst_test_base PRIVATE PkgConfig::gstreamer
    ${GST_LIBRARIES}
    ${GST_RTSP_LIBRARIES}
    pthread
    gstreamer-1.0
    gobject-2.0
    glib-2.0
    m
    z
)

#add link libs
target_link_libraries(gst_test_server PRIVATE PkgConfig::gstreamer
    ${GST_LIBRARIES}
    ${GST_RTSP_LIBRARIES}
    pthread
    gstreamer-1.0
    gobject-2.0
    glib-2.0
    m
    z
)