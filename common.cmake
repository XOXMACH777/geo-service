# from https://github.com/grpc/grpc/blob/v1.66.0/examples/cpp/cmake/common.cmake

option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
set(_GRPC_PYTHON_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_python_plugin>)

find_package(CURL CONFIG REQUIRED)
message(STATUS "Using CURL ${CURL_VERSION}")
set(_CURL_LIBCURL CURL::libcurl)

find_package(RapidJSON CONFIG REQUIRED)
message(STATUS "Using RapidJSON ${RapidJSON_VERSION}")
set(_RAPIDJSON rapidjson)
