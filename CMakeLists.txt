cmake_minimum_required(VERSION 3.13.0)

project(PsythonRenderer)

find_package(PythonExtensions REQUIRED)
find_library(SDL2_LIB sdl2)
find_path(SDL2_INCLUDE SDL.h)

add_library(_PsythonRenderer MODULE "")
target_sources(_PsythonRenderer
    PRIVATE
        PsythonRenderer/_PsythonRenderer.cpp
        PsythonRenderer/PsythonWindow.hpp
		PsythonRenderer/PsythonWindow.cpp
        PsythonRenderer/PsythonRendererCore.hpp
		PsythonRenderer/PsythonRendererCore.cpp
		PsythonRenderer/PsythonEvent.hpp
		PsythonRenderer/PsythonEvent.cpp
		PsythonRenderer/Tools/StackAllocator.hpp
		PsythonRenderer/Tools/StackAllocator.cpp
		PsythonRenderer/Tools/MemoryPool.hpp
		PsythonRenderer/Tools/MemoryPool.cpp
		PsythonRenderer/Tools/Alignment.hpp
)       
python_extension_module(_PsythonRenderer)
target_link_libraries(_PsythonRenderer "${SDL2_LIB}")
target_include_directories(_PsythonRenderer PRIVATE "${SDL2_INCLUDE}")
if (MSVC)
    target_compile_options(_PsythonRenderer PRIVATE /std:c++17)
endif()

install(TARGETS _PsythonRenderer LIBRARY DESTINATION PsythonRenderer)
install(FILES $<TARGET_PDB_FILE:_PsythonRenderer> DESTINATION PsythonRenderer OPTIONAL)