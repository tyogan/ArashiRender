file(GLOB_RECURSE SOURCE *.h *.cpp)

if (MSVC)
    add_definitions(-DUTILITY_API=__declspec\(dllexport\))
else()
    add_definitions(-DUTILITY_API=)
endif()

add_library(Utility SHARED ${SOURCE})				   