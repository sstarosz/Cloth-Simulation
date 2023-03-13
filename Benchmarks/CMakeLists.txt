cmake_minimum_required(VERSION 3.24)


 project(Cloth_Simulation_Benchmarks
		 VERSION 0.0.1
 		 DESCRIPTION "Benchmarks created for the purpose of this software" 
		 LANGUAGES CXX)


 find_package(benchmark REQUIRED CONFIG)



 ## Benchmark Example
 add_executable(PerformanceTests "Point3DBenchmark.cpp")

 set_target_properties(PerformanceTests PROPERTIES VS_DEBUGGER_COMMAND "--benchmark_out_format=json --benchmark_out=test.json")
 target_compile_features(PerformanceTests PRIVATE cxx_std_20)
 target_link_libraries(PerformanceTests 
						PUBLIC
						benchmark::benchmark
						Geometry)

