#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <IO/OBJImporter.hpp>

#include <chrono>

namespace st::io::unittests {
class ObjImporterTests : public testing::Test {

public:
    ObjImporterTests()
    {
    }

protected:
    ObjImporter objImporter;
};

TEST_F(ObjImporterTests, ReadObjFile)
{

    auto start = std::chrono::steady_clock::now();

    objImporter.readFromFile("Cube.obj");
    objImporter.printData();







    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"<< "\n";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << "\n";
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s" << "\n";
}

TEST_F(ObjImporterTests, Validate_ReadedData)
{
    objImporter.readFromFile("Cube.obj");
    auto vertices = objImporter.getGeometry();
    auto indeces = objImporter.getIndicesVector();
    //Cube
    const std::vector<Vertex> cube_mock = {
        //Front
        { { -0.5f, -0.5f, 0.5f }, { 0.375F, 0.00F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },
        { {  0.5f, -0.5f, 0.5f }, { 0.625F, 0.00F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },
        { {  0.5f,  0.5f, 0.5f }, { 0.625F, 0.25F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },
        { { -0.5f,  0.5f, 0.5f }, { 0.375F, 0.25F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },

        //Top
        { { -0.5f, 0.5f,  0.5f }, { 0.375F, 0.25F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },
        { {  0.5f, 0.5f,  0.5f }, { 0.625F, 0.25F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },
        { {  0.5f, 0.5f, -0.5f }, { 0.625F, 0.50F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },
        { { -0.5f, 0.5f, -0.5f }, { 0.375F, 0.50F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },

        //Back
        { { -0.5f,  0.5f, -0.5f }, { 0.375F, 0.50F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },
        { {  0.5f,  0.5f, -0.5f }, { 0.625F, 0.50F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },
        { {  0.5f, -0.5f, -0.5f }, { 0.625F, 0.75F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },
        { { -0.5f, -0.5f, -0.5f }, { 0.375F, 0.75F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },

        //Bottom
        { { -0.5f, -0.5f, -0.5f }, { 0.375F, 0.75F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },
        { {  0.5f, -0.5f, -0.5f }, { 0.625F, 0.75F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },
        { {  0.5f, -0.5f,  0.5f }, { 0.625F, 0.00F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },
        { { -0.5f, -0.5f,  0.5f }, { 0.375F, 0.00F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },

        // Right
        { { 0.5f, -0.5f,  0.5f }, { 0.625F, 0.00F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },
        { { 0.5f, -0.5f, -0.5f }, { 0.875F, 0.00F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },
        { { 0.5f,  0.5f, -0.5f }, { 0.875F, 0.25F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },
        { { 0.5f,  0.5f,  0.5f }, { 0.375F, 0.75F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },

        // Left
        { { -0.5f, -0.5f, -0.5f }, { 0.125F, 0.00F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
        { { -0.5f, -0.5f,  0.5f }, { 0.375F, 0.00F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
        { { -0.5f,  0.5f,  0.5f }, { 0.375F, 0.25F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
        { { -0.5f,  0.5f, -0.5f }, { 0.125F, 0.25F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
    };

    const std::vector<uint32_t> indices_mock = {
        0, 1, 3, 3, 2, 0,

        2, 3, 5, 5, 4, 2,

        7, 6, 4, 4, 5, 7,

        6, 7, 1, 1, 0, 6,

        1, 7, 5, 5, 3, 1,

        6, 0, 2, 2, 4, 6

    };


    //EXPECT_EQ(cube_mock, vertices);
    //EXPECT_EQ(indices_mock, indeces);



}


}