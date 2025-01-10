cmake_minimum_required(VERSION 3.18)
project(libvotv)

# Collect all header files
file(GLOB_RECURSE HEADER_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Create interface library (header-only)
add_library(${PROJECT_NAME} INTERFACE)

# Add header files to the target (makes them show up in Visual Studio)
target_sources(${PROJECT_NAME} INTERFACE ${HEADER_FILES})

# Add include directories
target_include_directories(${PROJECT_NAME} 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link UE4SS for the headers
target_link_libraries(${PROJECT_NAME} 
    INTERFACE 
        UE4SS
)

# IDE source group (optional, but helps organization in Visual Studio)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})