cmake_minimum_required(VERSION 3.4)

project(ArashiRender)

set_property(GLOBAL PROPERTY USE_FOLDERS ON) 

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories("dependency/include" "dependency/eigen-eigen-bdd17ee3b1b3")
link_directories("dependency/libs"  ${ASSIMP_LIBRARY_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/Render)
add_subdirectory(src/Editor)
add_subdirectory(src/Utility)
add_subdirectory(src/Core)

if (MSVC)
	add_definitions(-DRENDER_API=__declspec\(dllimport\))
	add_definitions(-DEDITOR_API=__declspec\(dllimport\))
	add_definitions(-DUTILITY_API=__declspec\(dllimport\))
	add_definitions(-DCORE_API=__declspec\(dllimport\))
else()
	add_definitions(-DRENDER_API=)
	add_definitions(-DEDITOR_API=)
	add_definitions(-DUTILITY_API=)	
	add_definitions(-DCORE_API=)
endif()

add_executable(ArashiRender_exe src/main.cpp)
add_dependencies(ArashiRender_exe Render Editor Utility Core)
target_link_libraries(ArashiRender_exe Render Editor Utility Core)