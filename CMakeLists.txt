# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building helloworld.

project(DistributedSystem)
cmake_minimum_required(VERSION 3.22.1)
set (CMAKE_CXX_STANDARD 20)

project(Entity C CXX)

add_subdirectory("network_simulator")

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

add_executable(entity main.cpp ${hw_proto_srcs})
target_link_libraries(entity protobuf::libprotobuf)

