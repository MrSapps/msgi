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
    pcx.cpp
    pcx.hpp
    Map.cpp
    Map.hpp
    Menu.cpp
    Menu.hpp
    Kmd.cpp
    Kmd.hpp
    Font.cpp
    Font.hpp
    masher_tables.hpp
    Masher.cpp
    Masher.hpp
    Table_665A3C.cpp
    Table_665A3C.hpp
    WinMain.cpp
    WinMain.hpp)

target_include_directories(MgsLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_compile_definitions(MgsLib PRIVATE "_CRT_SECURE_NO_WARNINGS")

if (MSVC)
    target_compile_options(MgsLib PRIVATE /W4 /wd4996)
endif()

SET_TARGET_PRECOMPILED_HEADER(MgsLib stdafx.h stdafx.cpp)

target_compile_features(MgsLib
    PRIVATE cxx_strong_enums
    PRIVATE cxx_static_assert
    PRIVATE cxx_lambdas
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    PRIVATE cxx_decltype
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_link_libraries(MgsLib
    Detours
    EasyLogging++
    #DxOld
    gmock
    zlib
    )

export(TARGETS MgsLib FILE MgsLib.cmake)
