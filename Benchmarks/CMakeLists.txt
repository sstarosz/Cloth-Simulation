 cmake_minimum_required(VERSION 3.19)

 project(Cloth_Simulation_Benchmark
 		 DESCRIPTION "Benchmarks created for the purpose of this software" 
		 VERSION 0.0.1
		 LANGUAGES CXX)

 list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/Conan/)

 find_package(benchmark REQUIRED CONFIG)



 add_executable(PerformanceTests "Point3DBenchmark.cpp")

 set_target_properties(PerformanceTests PROPERTIES VS_DEBUGGER_COMMAND "--benchmark_out_format=json --benchmark_out=test.txt")
 target_compile_features(PerformanceTests PRIVATE cxx_std_20)
 target_link_libraries(PerformanceTests 
						PUBLIC
						benchmark::benchmark
						Geometry)

