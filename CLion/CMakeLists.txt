cmake_minimum_required(VERSION 3.17)
project(EMWaves)

set(CMAKE_CXX_STANDARD 14)
include_directories(${PROJECT_SOURCE_DIR}/godot-headers)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../Godot/gdnative/windows)

include_directories(
        ${PROJECT_SOURCE_DIR}/src/
)
set(SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/WVS/fields.cpp
        ${PROJECT_SOURCE_DIR}/src/math/mathc.c
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/simple.c
)

add_library(waves SHARED ${SOURCE_FILES} ${PROJECT_SOURCE_DIR}/godot-headers/gdnative_api_struct.gen.h)