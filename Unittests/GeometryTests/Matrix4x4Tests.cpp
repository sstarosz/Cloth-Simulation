#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Geometry/Matrix4x4.hpp>


#include <chrono>

namespace st::geometry::unittests {


    TEST(MatrixTests, MatrixMultiplication)
    {
        Matrix4x4 A{ 5, 7,  9, 10,
                     2, 3,  3, 8,
                     8, 10, 2, 3,
                     3, 3,  4, 8 };

        Matrix4x4 B{ 3, 10, 12, 18,
                     12, 1,  4,  9,
                     9, 10, 12,  2,
                     3, 12,  4, 10 };

        Matrix4x4 Result{ 210, 267, 236, 271,
                          93,  149, 104, 149,
                          171, 146, 172, 268,
                          105, 169, 128, 169 };

        auto start = std::chrono::steady_clock::now();

        Matrix4x4 C = A * B;

        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << "\n";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << "\n";
        std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s" << "\n";



        EXPECT_EQ(Result, C);
    }


}