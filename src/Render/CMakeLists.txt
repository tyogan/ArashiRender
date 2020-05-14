file(GLOB_RECURSE SOURCE *.h *.cpp *.c *.cc)

file(GLOB_RECURSE GLSL *.glsl)  
source_group("shader" FILES ${GLSL})


find_package (embree 3 REQUIRED)
include_directories(
	${EMBREE_INCLUDE_DIRS}
)

if (MSVC)
    add_definitions(-DRENDER_API=__declspec\(dllexport\))
else()
    add_definitions(-DRENDER_API=)
endif()

add_library(Render SHARED ${SOURCE} ${GLSL})
add_custom_command(TARGET Render
                   PRE_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/shader
                       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shader)  			   
target_link_libraries(Render glfw3 ${ASSIMP_LIBRARIES} ${EMBREE_LIBRARIES})