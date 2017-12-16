add_executable(GCLASM GclAsm.cpp)

target_include_directories(GCLASM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(GCLASM
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
	target_compile_options(GCLASM PRIVATE /W4 /wd4996)
endif()

target_link_libraries(GCLASM MgsLib)

export(TARGETS GCLASM FILE GCLASM.cmake)
