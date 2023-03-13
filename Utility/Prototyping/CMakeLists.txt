


 add_executable(MemoryTypeCheck "MemoryTypeCheck.cpp" )
 target_compile_features(MemoryTypeCheck PRIVATE cxx_std_20)
 target_link_libraries(MemoryTypeCheck Vulkan::Vulkan)


 add_executable(TypeTraits "TypeTraits.cpp")
 target_compile_features(TypeTraits PRIVATE cxx_std_20)


  add_executable(Prototyping "Prototyping.cpp")
 target_compile_features(Prototyping PRIVATE cxx_std_20)