project(MgsLib VERSION 0.1 LANGUAGES CXX)

add_library(MgsLib
	Actor.cpp
	Actor.hpp
	Actor_Debug.cpp
	Actor_Debug.hpp
	Actor_Delay.cpp
	Actor_Delay.hpp
	Actor_GameD.cpp
	Actor_GameD.hpp
	Actor_Loader.cpp
	Actor_Loader.hpp
	Actor_Movie.cpp
	Actor_Movie.hpp
	Actor_Rank.cpp
	Actor_Rank.hpp
	File.cpp
	File.hpp
	Fs.cpp
	Fs.hpp
	Imports.cpp
	Imports.hpp
	Input.cpp
	Input.hpp
	LibDG.cpp
	LibDG.hpp
	LibGV.cpp
	LibGV.hpp
	logger.hpp
	MgsFunction.cpp
	MgsFunction.hpp
	Psx.cpp
	Psx.hpp
	Renderer.cpp
	Renderer.hpp
	ResourceNameHash.cpp
	ResourceNameHash.hpp
	Script.cpp
	Script.hpp
	Sound.cpp
	Sound.hpp
	stdafx.cpp
	stdafx.h
	System.cpp
	System.hpp
	targetver.h
	Task.cpp
	Task.hpp
	Timer.cpp
	Timer.hpp
	types.hpp
	WinMain.cpp)

target_include_directories(MgsLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_compile_definitions(MgsLib PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_compile_features(MgsLib
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_link_libraries(MgsLib
    Detours
	EasyLogging++
	#DxOld
	gmock
	)

export(TARGETS MgsLib FILE MgsLib.cmake)
