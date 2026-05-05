cmake_minimum_required(VERSION 3.18.1)

# Enable ASM language support to compile .s files
enable_language(ASM)

# Native library name
project(ptanative)

# Add the generated assembly file and the JNI bridge
add_library(
    ptanative
    SHARED
    pta_jni_bridge.cpp
    generated_logic.s
)

# Find required libraries
find_library(
    log-lib
    log
)

# Link libraries
# Added jnigraphics for Bitmap support and android for general android APIs
target_link_libraries(
    ptanative
    android
    jnigraphics
    ${log-lib}
)

# Include directories
target_include_directories(ptanative PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set compilation flags for C and C++ only
# This prevents ASM compilation from failing due to unused flags like -DANDROID
target_compile_options(ptanative PRIVATE
    $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-Wall -Werror>
)

# Enable NEON instructions only for 32-bit ARM (armeabi-v7a)
# arm64-v8a (AArch64) always has NEON, and the -mfpu=neon flag is an error there.
if(ANDROID_ABI STREQUAL armeabi-v7a)
    target_compile_options(ptanative PRIVATE -mfpu=neon)
endif()
