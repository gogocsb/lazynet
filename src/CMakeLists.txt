include_directories(${CMAKE_CURRENT_SOURCE_DIR}/protocol)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/handy)

# --- SRC handy
set(handy_LINKER_LIBS "")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/handy handy_SRCS)
add_definitions(-DOS_LINUX)
add_library(handy STATIC ${handy_SRCS})
target_link_libraries(handy ${handy_LINKER_LIBS})

# --- SRC codec
set(codec_LINKER_LIBS "")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/codec codec_SRCS)
add_library(codec STATIC ${codec_SRCS})
target_link_libraries(codec ${codec_LINKER_LIBS})

# --- SRC manager 



# --- SRC serivce 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/service)

# --- SRC utility
