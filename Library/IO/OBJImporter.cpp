#include "OBJImporter.hpp"

#include <fstream>
#include <iostream>

namespace st::io {

	ObjImporter::ObjImporter()
	{
	}

	ObjImporter::~ObjImporter()
	{
    }

    void ObjImporter::readFromFile(const std::filesystem::path& pathToObjFile)
    {

        if (std::filesystem::is_regular_file(pathToObjFile))
        {
            if (pathToObjFile.has_extension())
            {
                if (pathToObjFile.extension().string() != ".obj") 
                {

                }
            }

            if (pathToObjFile.has_filename()) 
            {

            }
        }


        std::ifstream objFile(pathToObjFile);

        if (!objFile)
        {
            std::cout << "Faile to read a file" << std::endl;
            return;
        }

        objFile.


    }

}