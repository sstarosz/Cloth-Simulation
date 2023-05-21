#ifndef IO_IMPORTERPROXY_HPP
#define IO_IMPORTERPROXY_HPP


#include <Geometry/Vertex.hpp>
#include <vector>
#include <filesystem>
#include "OBJImporter.hpp"


namespace st::io
{
  
//TODO - Clean content of previously readed files before read new one
class ImporterProxy
{

public:
  void readFile(const std::filesystem::path& pathToObjFile);


  // pos(vec3), color(vec3) texCord vec2
  std::vector<geometry::Vertex> getVertices();
  std::vector<geometry::Indices> getIndices();

private:

  //TODO
  //Get Shape
  //Get Object

//Change it to more generic importer?
    ObjImporter m_objImporter;

};

}

#endif //IO_IMPORTERPROXY_HPP
