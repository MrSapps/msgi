add_executable(Tests 
	ActorUT.cpp
	Main.cpp
	ResourceNameHashUT.cpp
	ScriptUT.cpp
	stdafx.cpp
	stdafx.h
	SystemUT.cpp
	targetver.h
	TaskUT.cpp
)

target_include_directories(Tests PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(Tests
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_link_libraries(Tests MgsLib)

export(TARGETS Tests FILE Tests.cmake)
