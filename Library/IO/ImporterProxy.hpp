#ifndef IO_IMPORTERPROXY_HPP
#define IO_IMPORTERPROXY_HPP



#include <Geometry/Vertex.hpp>
#include <vector>
#include <filesystem>


namespace st::io 
{
class ImporterProxy {

    void readFile(const std::filesystem::path& pathToObjFile);



    // pos(vec3), color(vec3) texCord vec2
    std::vector<geometry::Vertex> getVertices();
    std::vector<geometry::Indices> getIndices();
    

    //TODO
    //Get Shape
    //Get Object
};

}

#endif //IO_IMPORTERPROXY_HPP

