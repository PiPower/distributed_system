project(DistributedSystem)
cmake_minimum_required(VERSION 3.22.1)
set (CMAKE_CXX_STANDARD 20)

project(Entity C CXX)

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")


# Proto file
get_filename_component(hw_proto "${CMAKE_CURRENT_BINARY_DIR}/../proto/messages.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/../proto/messages.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/../proto/messages.pb.h")


add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}"
      COMMAND "protoc"
      ARGS --cpp_out "${hw_proto_path}"
        -I "${hw_proto_path}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")


include_directories("/home/mateusz/grpc_lib/include")

add_subdirectory("network_simulator")

add_executable(entity main.cpp ${hw_proto_srcs})
target_link_libraries(entity client protobuf::libprotobuf)

