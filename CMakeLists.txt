cmake_minimum_required(VERSION 3.4)

project(ArashiRender)

include_directories("dependency/include")
link_directories("dependency/libs")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/Render)
if (MSVC)
	add_definitions(-DRENDER_API=__declspec\(dllimport\))
else()
	add_definitions(-DRENDER_API=) 
endif()

add_executable(ArashiRender_exe src/main.cpp)
add_dependencies(ArashiRender_exe Render)
target_link_libraries(ArashiRender_exe Render)

