include_directories(../src/common)
include_directories(../src)

set(md5test_SRCS
    md5test.cc
    )

add_executable(md5test ${md5test_SRCS})
target_link_libraries(md5test hl++)

set(codeccli_SRCS
    codec-cli.cc
    )
add_executable(codeccli ${codeccli_SRCS})
target_link_libraries(codeccli handy
                      pthread)

set(DS_SRCS
    svr.cc
    )

add_executable(DS ${DS_SRCS})
target_link_libraries(DS
                    handy
                    proto
                    manager
                    glog
                    service
                    codec
                    pthread)
#set(MS_SRCS
#    ms.cc
#    )


#add_executable(MS ${MS_SRCS})
#target_link_libraries(MS
#                    handy
#                    proto
#                    manager
#                    glog
#                    service
#                    codec
#                    pthread)
