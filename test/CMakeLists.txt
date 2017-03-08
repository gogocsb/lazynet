set(md5test_SRCS
    md5test.cc
    )

add_executable(md5test ${md5test_SRCS})
target_link_libraries(md5test hl++)

set(codecsvr_SRCS
    codec-svr.cc
    )
add_executable(codecsvr ${codecsvr_SRCS})
target_link_libraries(codecsvr handy
                      pthread)
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
