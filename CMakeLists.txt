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

# ImGui source files
set(IMGUI_DIR vendor/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Paho MQTT setup
add_subdirectory(vendor/paho.mqtt.c EXCLUDE_FROM_ALL) # Adjust folder name if needed
set(PAHO_MQTT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/vendor/paho.mqtt.c/src")
set(PAHO_MQTT_LIBRARIES paho-mqtt3c)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_ttf)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3)
target_link_libraries(vendor INTERFACE SDL3_ttf::SDL3_ttf)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
	# ${SDL3_INCLUDE_DIRS}
	${IMGUI_DIR}
	${IMGUI_DIR}/backends
	# ${SDL_TTF_INCLUDE_DIRS}
	${PAHO_MQTT_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
)

# Link SDL3
target_link_libraries(${PROJECT_NAME} PRIVATE
	# ${SDL3_LIBRARIES}
	${PAHO_MQTT_LIBRARIES}
	${OPENGL_LIBRARIES}
	# ${SDL_TTF_LIBRARIES}
	${GLUT_LIBRARY}
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

# Link OpenGL (required for ImGui OpenGL backend)
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
else()
    message(FATAL_ERROR "OpenGL not found. Please install it and try again.")
endif()

# Platform-specific settings
if(WIN32)
    message(STATUS "Configuring for Windows")
elseif(UNIX)
    message(STATUS "Configuring for Linux/Unix")
endif()

# Compiler flags (optional customization)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
