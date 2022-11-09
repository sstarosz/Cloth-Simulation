#include "ImporterProxy.hpp"
#include "OBJImporter.hpp"

namespace st::io {

void ImporterProxy::readFile(const std::filesystem::path& pathToObjFile)
{

	if (pathToObjFile.extension() == ".obj")
	{
        ObjImporter objImporter;
        objImporter.readFromFile(pathToObjFile);


        //ObjImporter::readFromFile(pathToObjFile);
	}

}

std::vector<geometry::Vertex> ImporterProxy::getVertices()
{
    return std::vector<geometry::Vertex>();
}


}


