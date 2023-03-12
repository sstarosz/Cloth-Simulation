#Global Configuration CMake File
cmake_minimum_required(VERSION 3.24)

project(Cloth_Simulation 
        VERSION 0.0.1 
        DESCRIPTION  "Cloth simulation project" 
        LANGUAGES  CXX)


#Project option
option(Build_Unit_Test  "Determine if build unit tests" ON)
option(Build_Benchmarks "Determine if build benchmarks" ON)
option(Asan_Enabled     "Build this target with AddressSanitizer" OFF) #TODO


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if (MSVC)
    #Turn on Clang tidy when compling view cl
    #set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,google-readability-*,modernize-*,performance-*,readability-*)
    set(Compiler_Flags "/Wall;/permissive-;")    
endif()


if(Asan_Enabled)
    if(MSVC)
        #TODO - Enable santanizer  -fno-omit-frame-pointer
        list(APPEND Compiler_Flags "/fsanitize=address")
    else()
        list(APPEND Compiler_Flags  "-fsanitize=address")
        list(APPEND Linker_Flags    "-fsanitize=address")
    endif()
endif()


set(USE_FOLDERS True)


#Project Dependicies
find_package(Qt6 CONFIG REQUIRED
             COMPONENTS Core
                        Gui
                        Widgets
                        CoreTools)

find_package(Stb REQUIRED)



#Windeployqt for coping qt dll liblary on windows
if(MSVC)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${QT6_INSTALL_PREFIX})
endif()



#Include project structures
add_subdirectory(Library)
add_subdirectory(Application)
add_subdirectory(Assets)


#add_subdirectory(Docs)

if(Build_Benchmarks)
    add_subdirectory(Benchmarks)
endif()

if(Build_Unit_Test)
    add_subdirectory(Unittests)
endif()

add_subdirectory(Utility)
