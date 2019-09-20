file(GLOB_RECURSE SOURCE *.h *.cpp *.c)

file(GLOB_RECURSE GLSL *.glsl)  
source_group("shader" FILES ${GLSL})

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
target_link_libraries(Render glfw3)