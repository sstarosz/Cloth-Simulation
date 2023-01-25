include(CTest)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Unittests/)


add_subdirectory(Common)
add_subdirectory(GeometryTests)
add_subdirectory(IOTests)
add_subdirectory(RendererTests)
add_subdirectory(ViewportTests)

