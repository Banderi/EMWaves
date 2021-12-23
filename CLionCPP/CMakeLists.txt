cmake_minimum_required(VERSION 3.17)
project(CLionCPP)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../Godot/gdnative_cpp/windows)
set(GDN_ROOT ${PROJECT_SOURCE_DIR}/gdn_3.x)

include_directories(
        ${GDN_ROOT}/include/
        ${GDN_ROOT}/include/core
        ${GDN_ROOT}/include/gen
        ${GDN_ROOT}/include/godot-headers
)
set(SOURCE_FILES
        ${GDN_ROOT}/src/init.cpp
)

add_library(wavescpp SHARED ${SOURCE_FILES})
target_link_libraries(wavescpp PUBLIC ${GDN_ROOT}/bin/libgodot-cpp.windows.release.64.a)
#set_target_properties(wavescpp PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#    target_link_libraries(wavescpp PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgodot-cpp.windows.release.64.a)
#else()
#    target_link_libraries(wavescpp ${PROJECT_SOURCE_DIR}/lib/libgodot-cpp.windows.debug.64.a)
#endif()
#target_link_libraries(wavescpp PRIVATE -static-libgcc -static-libstdc++)