cmake_minimum_required(VERSION 3.18)
project(libvotv)

# Create interface library (header-only)
add_library(${PROJECT_NAME} INTERFACE)

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