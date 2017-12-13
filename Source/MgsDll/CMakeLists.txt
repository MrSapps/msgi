add_library(MgsDll SHARED dllmain.cpp)

target_include_directories(MgsDll PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(MgsDll
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_link_libraries(MgsDll MgsLib)

export(TARGETS MgsDll FILE MgsDll.cmake)
