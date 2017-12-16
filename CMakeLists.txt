cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(msgi VERSION 0.1 LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()
add_subdirectory(3rdParty)
add_subdirectory(Source/MgsLib)
add_subdirectory(Source/MgsExe)
add_subdirectory(Source/MgsDll)
add_subdirectory(Source/GCLASM)
add_subdirectory(Source/Tests)
