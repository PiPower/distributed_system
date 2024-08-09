#!/bin/bash
PROTOBUF_PATH=/home/mateusz/grpc_lib/bin
export PATH=$PATH:${PROTOBUF_PATH}
LIBRARY_PATH=/home/mateusz/grpc_lib/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${LIBRARY_PATH}
PACKAGES_PATH=/home/mateusz/grpc_lib/lib/pkgconfig
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:${PACKAGES_PATH}

