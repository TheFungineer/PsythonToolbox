cmake_minimum_required(VERSION 3.4.0)

project(PsythonRenderer)

find_package(PythonExtensions REQUIRED)
find_library(SDL2_LIB sdl2)
find_path(SDL2_INCLUDE SDL.h)

add_library(_PsythonRenderer MODULE PsythonRenderer/_PsythonRenderer.cpp)
python_extension_module(_PsythonRenderer)
target_link_libraries(_PsythonRenderer "${SDL2_LIB}")
message( ${SDL2_INCLUDE} )
target_include_directories(_PsythonRenderer PRIVATE "${SDL2_INCLUDE}")

install(TARGETS _PsythonRenderer LIBRARY DESTINATION PsythonRenderer)