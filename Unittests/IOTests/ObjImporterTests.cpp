#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <IO/OBJImporter.hpp>

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
    objImporter.readFromFile("Cube.obj");

    ASSERT_TRUE(true);
}

}