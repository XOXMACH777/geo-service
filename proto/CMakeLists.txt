project(proto)

# Define list of protocol files
file(GLOB_RECURSE PROTOS LIST_DIRECTORIES false "*.proto")

# Define CMake target for a library with auto-generated files
# See https://github.com/grpc/grpc/blob/v1.66.0/examples/cpp/helloworld/CMakeLists.txt
add_library(${PROJECT_NAME})
target_link_libraries(
    ${PROJECT_NAME} 
    absl::check 
    ${_REFLECTION} 
    ${_GRPC_GRPCPP} 
    ${_PROTOBUF_LIBPROTOBUF})

# Auto-generate code
# See https://github.com/protocolbuffers/protobuf/blob/main/docs/cmake_protobuf_generate.md#grpc-example
# { C++
protobuf_generate( # Protobuf files .pb.h and .pb.cc
    TARGET ${PROJECT_NAME}
    LANGUAGE cpp
    PROTOS ${PROTOS}
    PROTOC_OUT_DIR "${CMAKE_HOME_DIRECTORY}/proto"
)

protobuf_generate( # grpc files .grpc.pb.h and .grpc.pb.cc
    TARGET ${PROJECT_NAME}
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    PROTOS ${PROTOS}
    PROTOC_OUT_DIR "${CMAKE_HOME_DIRECTORY}/proto"
)
# }

# { Python
protobuf_generate( # Protobuf files _pb2.py
    TARGET ${PROJECT_NAME}
    LANGUAGE python
    PROTOS ${PROTOS}
    PROTOC_OUT_DIR "${CMAKE_HOME_DIRECTORY}/proto"
)

protobuf_generate( # grpc files _pb2_grpc.py
    TARGET ${PROJECT_NAME}
    LANGUAGE grpc
    GENERATE_EXTENSIONS .py
    PLUGIN "protoc-gen-grpc=${_GRPC_PYTHON_PLUGIN_EXECUTABLE}"
    PROTOS ${PROTOS}
    PROTOC_OUT_DIR "${CMAKE_HOME_DIRECTORY}/proto"
)
# }
