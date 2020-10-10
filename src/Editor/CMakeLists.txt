file(GLOB_RECURSE SOURCE *.h *.cpp)

find_package (Qt5 REQUIRED COMPONENTS Core Widgets Gui)

if (MSVC)
    add_definitions(-DEDITOR_API=__declspec\(dllexport\))
	add_definitions(-DRENDER_API=__declspec\(dllimport\))
	add_definitions(-DCORE_API=__declspec\(dllimport\))
	add_definitions(-DUTILITY_API=__declspec\(dllimport\))
else()
    add_definitions(-DEDITOR_API=)
endif()

add_library(Editor SHARED ${SOURCE})	
add_dependencies(Editor Render Core Utility)			   
target_link_libraries(Editor PUBLIC Render Core Utility Qt5::Core Qt5::Widgets Qt5::Gui)
