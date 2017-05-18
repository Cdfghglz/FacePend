cmake_minimum_required(VERSION 2.6)
project(gl_pendulum)


# definitions / options
add_definitions(
        -DUSE_MOUSE=0
        -DTRACK_DEBUG=1

        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)


if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fext-numeric-literals")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lopenblas -fext-numeric-literals")
endif()


# opengl
find_package(OpenGL REQUIRED)
set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130)


# OpenBlas
if(WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
    include_directories(OpenBLAS_INCLUDE_DIR)
    set(ALL_LIBS ${ALL_LIBS} ${OpenBLAS_LIB})
endif ()


# OpenCV
if(WIN32)
    set(OpenCV_DIR $ENV{OpenCV_DIR})
endif()
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(ALL_LIBS ${ALL_LIBS} ${OpenCV_LIBS})


# dlib
find_package(dlib QUIET)
IF (dlib_FOUND)
#    message(STATUS "Using dlib-${dlib_VERSION}")
ELSE (dlib_FOUND)
    set(DLIB_IN_PROJECT_BUILD 1)
    message(STATUS "Dlib will be built from source")
    set(DLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")
    include(${DLIB_PATH}/dlib/cmake)
    include_directories(${DLIB_PATH}/dlib/include)
    set(dlib_LIBRARIES dlib::dlib)
ENDIF(dlib_FOUND)
set(ALL_LIBS ${ALL_LIBS} ${dlib_LIBRARIES})


# SDL2
if(WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindSDL2.cmake")

    find_package(SDL2 REQUIRED)

    message(STATUS "Found SDL2 ${MINGW} ${sdl2_MAJOR_VERSION}")
    # path correction for compatibility with linux
    string(LENGTH ${SDL2_INCLUDE_DIR} SDL2_INCLUDE_DIR_LEN)
    math(EXPR SDL2_INCLUDE_DIR_LEN "(${SDL2_INCLUDE_DIR_LEN}-4)")
    string(SUBSTRING ${SDL2_INCLUDE_DIR} 0 ${SDL2_INCLUDE_DIR_LEN} SDL2_INCLUDE_DIRS)
    include_directories(${SDL2_INCLUDE_DIR})
else()
    set(SDL2_LIBRARIES "-L${SDL2_LIBDIR}  -lSDL2")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
endif()
include_directories(${SDL2_INCLUDE_DIRS})
set(ALL_LIBS ${ALL_LIBS} ${SDL2_LIBRARIES})


# boost
find_package(Boost REQUIRED)
if (${Boost_MAJOR_VERSION} VERSION_LESS 1 OR ${Boost_MINOR_VERSION} VERSION_LESS 58)
    message(STATUS "Downloading odeint, since boost version too old.")
    ExternalProject_Add(
            ODEINT
            URL https://github.com/headmyshoulder/odeint-v2/archive/v2.6_boost1.60.zip
            SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/odeint_source
            INSTALL_COMMAND  ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
    )
    include_directories (${CMAKE_CURRENT_BINARY_DIR}/odeint_source/include)
else()
    add_custom_target(ODEINT) # dependency dummy
endif()
include_directories (${Boost_INCLUDE_DIRS})


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
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

