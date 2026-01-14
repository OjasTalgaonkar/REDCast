#!/bin/bash

PROJECT_NAME="REDCast"

# 1. Create the Workspace Root
mkdir -p $PROJECT_NAME && cd $PROJECT_NAME

# 2. Setup the C++ Core (The Engine)
mkdir -p core/{src,include,build,lib}
mkdir -p core/src/{capture,encoder,common}

# Create a more robust CMakeLists for C++
cat <<EOF > core/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(RadioEngine)

set(CMAKE_CXX_STANDARD 17)

# Search for FFmpeg (System-wide installation recommended)
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED importlib libavcodec)
pkg_check_modules(AVFORMAT REQUIRED importlib libavformat)
pkg_check_modules(AVUTIL REQUIRED importlib libavutil)

include_directories(include \${AVCODEC_INCLUDE_DIRS})
add_executable(radio_engine src/main.cpp)
target_link_libraries(radio_engine \${AVCODEC_LIBRARIES} \${AVFORMAT_LIBRARIES} \${AVUTIL_LIBRARIES})
EOF

# 3. Setup the Go Service (The Signal/Discovery)
mkdir -p network/{cmd,pkg,internal}

# Initialize Go module (Assumes Go is installed)
cd network
go mod init github.com/user/radio-network 2>/dev/null || touch go.mod
cd ..

# 4. Global Infrastructure
mkdir -p assets docs tests script
touch README.md .gitignore

# Add a Master .gitignore
cat <<EOF > .gitignore
# C++
core/build/
*.o
*.exe

# Go
network/bin/
vendor/

# OS
.DS_Store
.vscode/
EOF

echo "Hybrid C++/Go structure for $PROJECT_NAME created."