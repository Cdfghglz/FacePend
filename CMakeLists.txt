# CMake entry point
cmake_minimum_required(VERSION 2.6)
project(gl_pendulum)

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fext-numeric-literals")

find_package(dlib REQUIRED)
message(STATUS "Using dlib-${dlib_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# options
add_definitions(-DUSE_MOUSE=1)
add_definitions(-DTRACK_DEBUG=1)

# Compile external dependencies 
add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

include_directories(
        external/glfw-3.1.2/include/GLFW/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        headers/
        src/
        .
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        ${SDL2_LIBRARY} glfw
        GLEW_1130
        )

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

# HEADERS
set(SOURCE_FILES
        headers/common.h
        headers/camera.h
        headers/transform.h
        headers/stb_image.h
        headers/mesh.h
        headers/display.h
        headers/shader.h
        headers/obj_loader.h
        headers/texture.h
        headers/pendulum.h
        headers/detector.h
        headers/tracker.h
        )

# SOURCES
set(SOURCE_FILES ${SOURCE_FILES}
        src/main.cpp
        src/common.cpp
        src/mesh.cpp
        src/display.cpp
        src/shader.cpp
        src/obj_loader.cpp
        src/texture.cpp
        src/stb_image.c
        src/pendulum.cpp
        src/detector.cpp
        src/tracker.cpp
        )

add_executable(gl_pendulum ${SOURCE_FILES})

set(SDL2_LIBRARIES "-L${SDL2_LIBDIR}  -lSDL2")
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
set(ALL_LIBS ${ALL_LIBS} ${SDL2_LIBRARIES})
set(ALL_LIBS ${ALL_LIBS} ${OpenCV_LIBS})
set(ALL_LIBS ${ALL_LIBS} ${dlib_LIBRARIES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
        ${ALL_LIBS}
        )

# Xcode and Visual working directories
set_target_properties(gl_pendulum PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/")
create_target_launcher(gl_pendulum WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

