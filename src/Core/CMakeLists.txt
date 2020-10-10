file(GLOB_RECURSE SOURCE *.h *.cpp)

if (MSVC)
    add_definitions(-DCORE_API=__declspec\(dllexport\))
else()
    add_definitions(-DCORE_API=)
endif()

add_library(Core SHARED ${SOURCE})	
target_link_libraries(Core ${ASSIMP_LIBRARIES})
	