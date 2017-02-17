set(md5test_SRCS
    md5test.cc
    )

add_executable(md5test ${md5test_SRCS})
target_link_libraries(md5test hl++)
