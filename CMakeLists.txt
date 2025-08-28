cmake_minimum_required(VERSION 3.15)

# Define main project
project(geo)

# Include common settings
include(common.cmake)

# Set some common flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add auto-generated code as a separate library
add_subdirectory(proto)

# Define list of source code files
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false "src/*.cc")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false "src/*.h")

# Define CMake target for the project
# See https://github.com/grpc/grpc/blob/v1.66.0/examples/cpp/helloworld/CMakeLists.txt
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(
    ${PROJECT_NAME}
    proto
    absl::check
    absl::flags_parse
    absl::absl_log
    absl::log_initialize
    absl::log_globals
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    ${_CURL_LIBCURL}
    ${_RAPIDJSON})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_HOME_DIRECTORY}/proto")
