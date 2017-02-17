# --- SRC handy
set(handy_LINKER_LIBS "")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/handy handy_SRCS)
add_definitions(-DOS_LINUX)
add_library(handy STATIC ${handy_SRCS})
target_link_libraries(handy ${handy_LINKER_LIBS})


# --- SRC manager 

# --- SRC serivce 

# --- SRC utility
