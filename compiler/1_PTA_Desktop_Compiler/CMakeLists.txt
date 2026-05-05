cmake_minimum_required(VERSION 3.16)
project(PTA_Compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/optimizer.cpp
    src/mapper.cpp
    src/emitter.cpp
    src/utils.cpp
)

# Include directories
include_directories(src)

# Create the compiler executable
add_executable(pta_compiler ${SOURCES})

# Set output directory to make it easier to run
set_target_properties(pta_compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation (optional)
install(TARGETS pta_compiler
    DESTINATION bin
)

# Copy demo script to build directory
configure_file(
    scripts/demo_filter.tasm
    ${CMAKE_BINARY_DIR}/scripts/demo_filter.tasm
    COPYONLY
)

message("PTA Compiler configured successfully!")
message("Build with: cmake --build . --target pta_compiler")
message("Run with: ./bin/pta_compiler ../scripts/demo_filter.tasm")