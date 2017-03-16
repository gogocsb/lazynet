cmake_minimum_required(VERSION 2.8) 
##################################################################
project(lazynet)
if(CMAKE_BUILD_TYPE)
else()
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

# --- Option

option(USE_PROTO "Build with protocolbuf." ON)
option(USE_GLOG "Build with golg." ON)
option(USE_HASHLIB "Build with hashlib" ON)

# --- Compiler Flags

set(CXX_FLAGS 
    -g
    -std=c++11
    #-DVALGRIND
    #-Wall
    -Wextra
    -Werror
    #-Wconversion
    -Wno-format-security
    -Wno-unused-parameter
    #-Woverloaded-virtual
    -Wpointer-arith
    #-Wshadow
    -Wwrite-strings
    -march=native
    -rdynamic
    )

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}" )

# --- Output

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# --- Dependency

# ---- Boost
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
message ("  BOOST_ROOT: ${Boost_INCLUDE_DIRS}")

# ---- Protobuf 
if(USE_PROTO)
    find_package(Protobuf REQUIRED)
    add_subdirectory(protobuf)
    message ("  ADD Protobuf")
endif()

# ---- Glog
if(USE_GLOG)
    include_directories(/usr/local/include/glog)
    message ("  ADD Glog")
endif()


# ---- Hashlib
if(USE_HASHLIB)
    message("  ADD Hashlib")
endif()


# --- Subdirectory

add_subdirectory(src)
add_subdirectory(test)
