include_directories(${CMAKE_CURRENT_SOURCE_DIR}/protocol)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# --- SRC handy
set(handy_LINKER_LIBS "")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/handy handy_SRCS)
add_definitions(-DOS_LINUX)
add_library(handy STATIC ${handy_SRCS})
target_link_libraries(handy ${handy_LINKER_LIBS})

# --- SRC codec
set(codec_LINKER_LIBS 
    glog)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/codec codec_SRCS)
add_library(codec STATIC ${codec_SRCS})
target_link_libraries(codec ${codec_LINKER_LIBS})

# --- SRC manager 

set(manager_LINKER_LIBS "")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/manager manager_SRCS)
add_library(manager STATIC ${manager_SRCS})
target_link_libraries(manager ${manager_LINKER_LIBS})


# --- SRC serivce 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/service)
#set(manager_LINKER_LIBS
#    proto
#    handy)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/service service_SRCS)
#add_library(service STATIC ${service_SRCS})
#target_link_libraries(service ${service_LINKER_LIBS})
# --- SRC utility
