cmake_minimum_required(VERSION 3.4.0)

project(PsythonRenderer)

find_package(PythonExtensions REQUIRED)

add_library(_PsythonRenderer MODULE PsythonRenderer/_PsythonRenderer.cpp)
python_extension_module(_PsythonRenderer)
install(TARGETS _PsythonRenderer LIBRARY DESTINATION PsythonRenderer)