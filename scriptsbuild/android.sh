#!/bin/bash

set -e

export NDK="$HOME/.local/opt/android/android-ndk-r29"

mkdir -p _build/android

cmake -S . \
	-B _build/android \
	-DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-24 \
	-Dcm_plat=android 
cmake --build _build/android


echo "Copying to termux"
project_dir="/data/data/com.termux/files/home/projects/p02_and"
mkdir -p "$project_dir"
mkdir -p "$project_dir/out/lib/arm64-v8a"

cp _bin/Debug_android/libak.so "$project_dir/out/lib/arm64-v8a/"
cp _bin/Debug_android/libmain_android.so "$project_dir/out/lib/arm64-v8a/"
