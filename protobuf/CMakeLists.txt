cmake_minimum_required(VERSION 2.8)

file(GLOB PROTO_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)
if(PROTO_INPUT)
foreach(PROTO_FILE ${PROTO_INPUT})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    set(CUR_PROTO_GEN
        ${PROTO_NAME}.pb.h
        ${PROTO_NAME}.pb.cc
        )
    set(PROTO_GEN
        ${PROTO_GEN}
        ${CUR_PROTO_GEN}
        )
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_NAME}.proto)
    set_source_files_properties(${PROTO_NAME}.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion -Wno-shadow")
endforeach(PROTO_FILE ${PROTO_INPUT})

add_library(proto ${PROTO_GEN})
target_link_libraries(proto ${PROTOBUF_LIBRARIES} pthread)

add_custom_command(
    TARGET proto
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E make_directory ${PROJECT_SOURCE_DIR}/src/protocol
    )

foreach(PROTO_FILE ${PROTO_INPUT})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    add_custom_command(
        TARGET proto
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${PROTO_NAME}.pb.cc ${PROJECT_SOURCE_DIR}/src/protocol
        )
    add_custom_command(
        TARGET proto
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${PROTO_NAME}.pb.h ${PROJECT_SOURCE_DIR}/src/protocol
        )
endforeach(PROTO_FILE ${PROTO_INPUT})
endif()
