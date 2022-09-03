 
 
 
 add_custom_target(Copy_Assets_File 
  DEPENDS ${CMAKE_BINARY_DIR}/Assets
  )
 
  add_dependencies(Cloth_Simulation_App Copy_Assets_File)

 add_custom_command(TARGET Copy_Assets_File POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                             ${CMAKE_SOURCE_DIR}/Assets/Shaders/frag.spv
                             ${CMAKE_BINARY_DIR}/Assets/Shaders/frag.spv
                     COMMENT "Copy fragment shader"
                     )

 add_custom_command(TARGET Copy_Assets_File POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                             ${CMAKE_SOURCE_DIR}/Assets/Shaders/vert.spv
                             ${CMAKE_BINARY_DIR}/Assets/Shaders/vert.spv
                     COMMENT "Copy vertex shader"
                     )