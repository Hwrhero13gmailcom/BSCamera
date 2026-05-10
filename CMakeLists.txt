cmake_minimum_required(VERSION 3.22)
project(camera-offset-mod)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(COMPILE_ID camera-offset-mod_0_1_0)

include_directories(
    extern/includes
    extern/includes/beatsaber-hook/shared
    include
)

add_library(${COMPILE_ID} SHARED
    src/main.cpp
)

target_compile_options(${COMPILE_ID} PRIVATE
    -frtti
    -fexceptions
    -O2
)

target_link_libraries(${COMPILE_ID} PRIVATE
    -llog
    -landroid
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/libs/libcustom-types.so
)
