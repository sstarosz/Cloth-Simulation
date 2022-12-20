include(CTest)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Unittests)


add_subdirectory(Common)
add_subdirectory(GeometryTests)
add_subdirectory(IOTests)


#add_executable(MainTest TestMain.cpp "IOTest/ObjImporterTests.cpp")
#target_link_libraries(MainTest GTest::gtest GTest::gtest_main GTest::gmock)


add_executable(TestExplorer TestExplolerTest.cpp)
target_link_libraries(TestExplorer PRIVATE GTest::gtest)

#add_test(
#	NAME TestExplorerTest
#	COMMAND TestExplorer)

gtest_discover_tests(TestExplorer)