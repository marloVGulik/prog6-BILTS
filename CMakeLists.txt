cmake_minimum_required(VERSION 3.10)

# Project name and version
project(BILTS VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include all .cpp and .h files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Platform-specific settings
if(WIN32)
    message(STATUS "Configuring for Windows")
    # Add Windows-specific flags here if needed
elseif(UNIX)
    message(STATUS "Configuring for Linux/Unix")
    # Add Linux-specific flags here if needed
endif()

# Add custom include directories (if any)
# include_directories(include)

# Compiler flags (optional customization)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Add additional libraries or dependencies here if needed
# target_link_libraries(${PROJECT_NAME} some_library)
