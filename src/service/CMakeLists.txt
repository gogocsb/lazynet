set(service_SRCS
    DSServer.cc
    )
add_library(service  ${service_SRCS})
target_link_libraries(service
    handy
    proto
    manager)

