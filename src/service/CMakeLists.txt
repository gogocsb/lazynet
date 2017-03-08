set(service_SRCS
    DSServer.cc
    ./SessionService/SessionService.cc
    ./SessionService/StoreSession.cc
    ./SessionService/LoadSession.cc
    )
add_library(service  ${service_SRCS})
target_link_libraries(service
    handy
    proto
    manager)

