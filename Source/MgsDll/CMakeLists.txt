add_library(MgsDll SHARED dllmain.cpp)

target_include_directories(MgsDll PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(MgsDll
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(MgsDll PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(MgsDll PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
endif()

target_link_libraries(MgsDll MgsLib)

add_custom_command(TARGET MgsDll
                   POST_BUILD
                   COMMAND echo f | xcopy /s/y "$(OutDir)$(TargetName)$(TargetExt)" "C:\\Program Files (x86)\\Metal Gear Solid\\ebueula.dll"
                   COMMENT "Copying to game dir")

export(TARGETS MgsDll FILE MgsDll.cmake)
install(TARGETS MgsDll DESTINATION .)
