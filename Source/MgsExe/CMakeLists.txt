add_executable(MgsExe WIN32 MgsExe.cpp)

target_include_directories(MgsExe PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(MgsExe
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(MgsExe PROPERTIES LINK_FLAGS "/DYNAMICBASE:NO /FIXED /BASE:0x40000")
endif()

target_link_libraries(MgsExe MgsLib)

export(TARGETS MgsExe FILE MgsExe.cmake)
install(TARGETS MgsExe DESTINATION .)
