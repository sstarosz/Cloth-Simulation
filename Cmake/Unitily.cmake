
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#
#
#
#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#  if(_variableName MATCHES "^Qt6.*LIBRARIES")
#      message(STATUS "${_variableName}")
#      message(STATUS "\t${${_variableName}}")
#  endif()
#endforeach()

#get_target_property(QtCore_location Qt6::windeployqt LOCATION)
#message(${QtCore_location})



#add_custom_command(
#    TARGET Cloth_Simulation POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        $<TARGET_FILE:Qt6::Widgets>
#        $<TARGET_FILE_DIR:Cloth_Simulation>
#)