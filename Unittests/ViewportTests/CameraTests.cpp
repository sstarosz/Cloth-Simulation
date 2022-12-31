#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Viewport/Camera.hpp>


#include <iostream>

namespace st::viewport::unittests {

    void printEyeVector(const geometry::Vector3& eye)
    {
        std::cout <<"Eye: " << eye[0] << " " << eye[1] << " " << eye[2] << std::endl;
    }


    void printMatrix(const geometry::Matrix4x4& matrix)
    {
        std::cout << matrix[0] << "\t" << matrix[1] << "\t" << matrix[2] << "\t" << matrix[3] << std::endl;
        std::cout << matrix[4] << "\t" << matrix[5] << "\t" << matrix[6] << "\t" << matrix[7] << std::endl;
        std::cout << matrix[8] << "\t" << matrix[9] << "\t" << matrix[10] << "\t" << matrix[11] << std::endl;
        std::cout << matrix[12] << "\t" << matrix[13] << "\t" << matrix[14] << "\t" << matrix[15] << std::endl;
    }


    void orbit(float dx, float dy)
    {
        static Camera camera;
        std::cout << "-------------------" << dx << " " << dy << "-------------------" << std::endl;
        auto result = camera.orbitTest(dx, dy);
        camera.update();
        printEyeVector(result);
        std::cout << "-------------------------------------------------" << std::endl;
        printMatrix(camera.getViewMatrix());

        std::cout << "-------------------------------------------------" << std::endl;

        std::cout << std::endl;
    }


    TEST(CameraTests, CameraOrbit)
    {
        orbit(0.10F, 0.10F);
        orbit(0.25F, 0.25F);
        orbit(0.5F, 0.5F);

        orbit(0.0F, -1.0F);
        orbit(0.0F, -0.75F);
        orbit(0.0F, -0.5F);


        orbit(0.0,  0.5F);
        orbit(0.0F, 0.75F);
        orbit(0.0F, 1.0F);




    }



}