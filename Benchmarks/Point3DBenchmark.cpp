#include <benchmark/benchmark.h>
#include <Geometry/Point3D.hpp>
#include <iostream>

static void ST_Point3DAdd(benchmark::State& state)
{

    std::cout << sizeof(st::geometry::Point3D) << std::endl;

    for (auto _ : state)
    {
        st::geometry::Point3D result{ 0.0,0.0,0.0 };
        for (size_t i = 0; i < 10000; ++i)
        {
            result += st::geometry::Point3D(i, i, i);
        }
    }
}
// Register the function as a benchmark
BENCHMARK(ST_Point3DAdd);

BENCHMARK_MAIN();