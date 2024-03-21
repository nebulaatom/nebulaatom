#!/usr/bin/env bash

# Set current directory
source_dir="${PWD}"

# Create build directory
build_dir='./build'
if [ -d $build_dir ]; then
    echo "- Delete old build directory: ${build_dir}"
    rm -rf $build_dir
fi

echo "- Create building folder: ${build_dir}"
mkdir $build_dir

echo "- Entering folder: ${build_dir}"
cd $build_dir

# Configure
echo "- Configure Nebula Atom"
cmake ../ -DCMAKE_BUILD_TYPE=release
if [ "$?" != "0" ]; then
    exit -1
fi

# Build
echo "- Build Nebula Atom"
cmake --build .
if [ "$?" != "0" ]; then
    exit -1
fi

# Install
echo "- Install Nebula Atom"
sudo cmake --build . --target install

# Go back to the current directory
cd $source_dir