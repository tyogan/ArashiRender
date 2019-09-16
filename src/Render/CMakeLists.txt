file(GLOB_RECURSE SOURCE *.h *.cpp *.c)

if (MSVC)
    add_definitions(-DRENDER_API=__declspec\(dllexport\))
else()
    add_definitions(-DRENDER_API=)
endif()

add_library(Render SHARED ${SOURCE})
target_link_libraries(Render glfw3)