 cmake_minimum_required(VERSION 3.19)

 project(Utilities_Snipets
 		 DESCRIPTION "List of utilites snipets for prototyping and testing purpose" 
		 VERSION 0.0.1
		 LANGUAGES CXX)






 add_executable(MemoryTypeCheck "MemoryTypeCheck.cpp" )
 target_compile_features(MemoryTypeCheck PRIVATE cxx_std_20)
 target_link_libraries(MemoryTypeCheck Vulkan::Vulkan)


 add_executable(TypeTraits "TypeTraits.cpp")
 target_compile_features(TypeTraits PRIVATE cxx_std_20)
