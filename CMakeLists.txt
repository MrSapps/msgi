cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(msgi VERSION 0.1 LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(SET_TARGET_PRECOMPILED_HEADER Target PrecompiledHeader PrecompiledSource)
  if(MSVC)
     SET_TARGET_PROPERTIES(${Target} PROPERTIES COMPILE_FLAGS "/Yu${PrecompiledHeader}")
     set_source_files_properties(${PrecompiledSource} PROPERTIES COMPILE_FLAGS "/Yc${PrecompiledHeader}")
  endif(MSVC)
endfunction(SET_TARGET_PRECOMPILED_HEADER)

enable_testing()
add_subdirectory(3rdParty)
add_subdirectory(Source/MgsLib)
add_subdirectory(Source/MgsExe)
add_subdirectory(Source/MgsDll)
add_subdirectory(Source/GCLASM)
add_subdirectory(Source/Tests)

if(WIN32)
    # On Windows generate ZIP packages
    SET(CPACK_GENERATOR "ZIP")
endif()
INCLUDE(CPack)
