#Adding documentation generation

message("Generate Documents")

find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# Find all the public headers
file(GLOB_RECURSE App_Public_Header ${CMAKE_SOURCE_DIR}/Library/*.hpp)
message(!!! ${App_Public_Header})



set(Doxygen_Input_Dir ${CMAKE_SOURCE_DIR}/Library)
set(Doxygen_Output_Dir ${CMAKE_SOURCE_DIR}/Docs/Doxygen)
set(Doxygen_Index_File ${Doxygen_Output_Dir}/xml/index.xml)

#Template File
set(Doxyfile_In ${CMAKE_SOURCE_DIR}/Docs/Doxygen/Doxyfile.in)
set(Doxyfile_Out ${CMAKE_SOURCE_DIR}/Docs/Doxygen/Doxyfile)

#Replace variables inside @@ with the current values
configure_file(${Doxyfile_In} ${Doxyfile_Out} @ONLY)


file(MAKE_DIRECTORY ${Doxygen_Output_Dir}) #Doxygen won't create this for us
add_custom_command(OUTPUT ${Doxygen_Index_File}
        DEPENDS ${App_Public_Header}
        COMMAND ${DOXYGEN_EXECUTABLE} ${Doxyfile_Out}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Docs
        MAIN_DEPENDENCY ${Doxyfile_In}
        COMMENT "Generating docs using doxyfile")

add_custom_target(Doxygen DEPENDS ${Doxygen_Index_File})

set(Sphinx_Source ${CMAKE_SOURCE_DIR}/Docs/Sphinx/source)
set(Sphinx_Build ${CMAKE_SOURCE_DIR}/Docs/Sphinx/build)
set(Sphinx_Index_File ${Sphinx_Build}/index.html)


add_custom_command(OUTPUT ${Sphinx_Index_File}
        COMMAND ${SPHINX_EXECUTABLE} -b html
        # Tell Breathe where to find the Doxygen output
        ${Sphinx_Source} ${Sphinx_Build}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Docs
        DEPENDS
        # Other docs files you want to track should go here (or in some variable)
        ${Sphinx_Source}/index.rst
        ${Sphinx_Source}/usage.rst
        ${Doxygen_Index_File}
        MAIN_DEPENDENCY ${Sphinx_Source}/conf.py
        COMMENT "Generating documentation with Sphinx")

add_custom_target(Sphinx DEPENDS ${Sphinx_Index_File}

    SOURCES 
    ${Sphinx_Source}/index.rst
    ${Sphinx_Source}/usage.rst
)

# Add an install target to install the docs
#include(GNUInstallDirs)
#install(DIRECTORY ${Sphinx_Build}
#        DESTINATION ${CMAKE_INSTALL_DOCDIR})
