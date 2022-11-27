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

    void printGeometry(std::vector<Vertex>& data) const;
    void printIndices(std::vector<uint32_t>& data) const;

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
    printGeometry(vertices);


    auto indeces = objImporter.getIndicesVector();
    printIndices(indeces);


    int k = 0;

    //Cube
    const std::vector<Vertex> cube_mock = {
          //Front 0-3
          { { -0.5f, -0.5f, 0.5f }, { 0.375F, 1.0F - 0.00F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },
          { {  0.5f, -0.5f, 0.5f }, { 0.625F, 1.0F - 0.00F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },
          { {  0.5f,  0.5f, 0.5f }, { 0.625F, 1.0F - 0.25F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },
          { { -0.5f,  0.5f, 0.5f }, { 0.375F, 1.0F - 0.25F }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0F } },

          //Top 4-7
          { { -0.5f, 0.5f,  0.5f }, { 0.375F, 1.0F - 0.25F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },
          { {  0.5f, 0.5f,  0.5f }, { 0.625F, 1.0F - 0.25F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },
          { { -0.5f, 0.5f, -0.5f }, { 0.375F, 1.0F - 0.50F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },
          { {  0.5f, 0.5f, -0.5f }, { 0.625F, 1.0F - 0.50F }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0F } },

          //Back 8-11
          { { -0.5f,  0.5f, -0.5f }, { 0.375F, 1.0F - 0.50F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },
          { {  0.5f,  0.5f, -0.5f }, { 0.625F, 1.0F - 0.50F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },
          { {  0.5f, -0.5f, -0.5f }, { 0.625F, 1.0F - 0.75F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },
          { { -0.5f, -0.5f, -0.5f }, { 0.375F, 1.0F - 0.75F }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0F } },

          //Bottom 12-15
          { { -0.5f, -0.5f, -0.5f }, { 0.375F, 1.0F - 0.75F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },
          { {  0.5f, -0.5f, -0.5f }, { 0.625F, 1.0F - 0.75F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },
          { {  0.5f, -0.5f,  0.5f }, { 0.625F, 1.0F - 1.00F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },
          { { -0.5f, -0.5f,  0.5f }, { 0.375F, 1.0F - 1.00F }, { 0.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 1.0F } },

          // Right 16-19
          { { 0.5f, -0.5f,  0.5f }, { 0.625F, 1.0F - 0.00F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },
          { { 0.5f, -0.5f, -0.5f }, { 0.875F, 1.0F - 0.00F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },
          { { 0.5f,  0.5f, -0.5f }, { 0.875F, 1.0F - 0.25F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },
          { { 0.5f,  0.5f,  0.5f }, { 0.625F, 1.0F - 0.25F }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0F } },

          // Left 20-23
          { { -0.5f, -0.5f, -0.5f }, { 0.125F, 1.0F - 0.00F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
          { { -0.5f, -0.5f,  0.5f }, { 0.375F, 1.0F - 0.00F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
          { { -0.5f,  0.5f,  0.5f }, { 0.375F, 1.0F - 0.25F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
          { { -0.5f,  0.5f, -0.5f }, { 0.125F, 1.0F - 0.25F }, { 1.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0F } },
    };

    const std::vector<uint32_t> indices_mock =
    {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 5, 7, 6,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
    };


    //EXPECT_EQ(cube_mock, vertices);
    //EXPECT_EQ(indices_mock, indeces);



}


void ObjImporterTests::printGeometry(std::vector<Vertex>& data) const
{
    for (const auto& vertex : data)
    {
        std::cout << "Position: { " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << " } "
                  << "Texture:  { " << vertex.textureCord.u << ", " << vertex.textureCord.v << " } "  
                  << "Color:    { " << vertex.color.x << ", " << vertex.color.y << ", " << vertex.color.z << " } "
                  << "Normal:   { " << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << " }\n";
    }

}

void ObjImporterTests::printIndices(std::vector<uint32_t>& data) const
{
    std::cout << "Indices: {\n";
    for (uint64_t index = 0; const auto& vertex : data)
    {
        if (index == 6)
        {
            std::cout << "\n";
            index = 0;
        }

        std::cout << vertex << ", ";

        index++;
    }
    std::cout << "}\n";
}

}