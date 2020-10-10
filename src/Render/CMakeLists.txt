file(GLOB_RECURSE SOURCE *.h *.cpp *.cc *.c)

file(GLOB_RECURSE GLSL *.glsl)  
source_group("shader" FILES ${GLSL})

file(GLOB_RECURSE GLSL_MTL *.mtl)  
source_group("mtl" FILES ${GLSL_MTL})

find_package (embree 3 REQUIRED)
include_directories(
	${EMBREE_INCLUDE_DIRS}
)

if (MSVC)
    add_definitions(-DRENDER_API=__declspec\(dllexport\))
	add_definitions(-DCORE_API=__declspec\(dllimport\))
	add_definitions(-DUTILITY_API=__declspec\(dllimport\))
else()
    add_definitions(-DRENDER_API=)
endif()

add_library(Render SHARED ${SOURCE} ${GLSL} ${GLSL_MTL})
add_custom_command(TARGET Render
                   PRE_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/shader
                       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shader)
add_custom_command(TARGET Render
                   PRE_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/mtl
                       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mtl)  		
add_dependencies(Render Core Utility)					   
target_link_libraries(Render glfw3 Core Utility ${ASSIMP_LIBRARIES} ${EMBREE_LIBRARIES})