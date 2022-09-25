include(CTest)
enable_testing()

find_package(GTest CONFIG REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Unittests)


add_subdirectory(Common)
add_subdirectory(IOTests)

#add_executable(MainTest TestMain.cpp "IOTest/ObjImporterTests.cpp")
#target_link_libraries(MainTest GTest::gtest GTest::gtest_main GTest::gmock)

#add_test(AllTestsApplication MainTest)