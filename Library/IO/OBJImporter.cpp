#include "OBJImporter.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <charconv>
#include <cassert>

namespace st::io {

/*
Notes
Do not support free points and lines only faces

free form geometr not supported yet

*/

// TODO Grouping
// TODO Display/render attributes(materials)

ObjImporter::ObjImporter(): m_geoVertices() { }

void ObjImporter::readFromFile(const std::filesystem::path& pathToObjFile)
{
  // #Check if the file is valid
  if (std::filesystem::is_regular_file(pathToObjFile))
  {
    if (pathToObjFile.has_extension())
    {
      if (pathToObjFile.extension().string() != ".obj")
      {
        throw std::runtime_error("Wrong file format");
      }
    }

    // if (pathToObjFile.has_filename())
    //{
    // }
  }

  // Read data from file
  // To-Do check if better to read whole file at once
  // Or is better to read be specyfic chunk (Performense Tests)
  std::ifstream objFile(pathToObjFile);
  if (!objFile)
  {
    throw std::runtime_error("Failed to read .obj file");
  }

  const auto fileSize = std::filesystem::file_size(pathToObjFile);
  std::string fileData(fileSize, '\0');
  objFile.read(fileData.data(), fileSize);

  std::stringstream localStream(fileData);

  std::string operatior;
  while (localStream >> operatior)
  {
    // Remove space at the beginig;
    localStream >> std::ws;

    if (operatior == "v") // Vertex position
    {
      static float x = 0.0F;
      static float y = 0.0F;
      static float z = 0.0F;

      localStream >> x >> y >> z;
      m_geoVertices.emplace_back(x, y, z);
    }
    else if (operatior == "vt") // Texture cordinate
    {
      static float u = 0.0F;
      static float v = 0.0F;

      localStream >> u >> v;
      m_textCord.emplace_back(u, v);
    }
    else if (operatior == "vn") // Vertex normal
    {
      static float i = 0.0F;
      static float j = 0.0F;
      static float k = 0.0F;

      localStream >> i >> j >> k;
      m_normalVertices.emplace_back(i, j, k);
    }
    else if (operatior == "f") // Face definition
    {

        // TODO add option for skiping texture cord or normal

        std::string face;
        std::getline(localStream, face);
        uint32_t geoId { 0 };
        uint32_t texId { 0 };
        uint32_t norId { 0 };
        char separator { '/' };
        faceID faceIndexes;
        for (const auto& eleOfFace : std::views::split(face, ' '))
        {
          std::string vertex(eleOfFace.begin(), eleOfFace.end());
          std::stringstream localStream(vertex);
          localStream >> std::ws >> geoId >> separator >> texId >> separator >> norId;

          //Obj file format index from 1 so we need to substrat 1 to indexing from 0
          faceIndexes.emplace_back(geoId - 1, texId - 1, norId - 1);
        }
        m_faces.push_back(faceIndexes);
        // std::cout << face << "\n";
    }

    objFile.close();
    // Parse Readed data
    // Vertexs
    // Uv
    // Normal
    // Materials
  }
}

void ObjImporter::printData() const
{

  // Vertex
  std::cout << "\nVertex Position:\n";
  for (const auto& vert : m_geoVertices)
  {
    std::cout << "(" << vert.x << " " << vert.y << " " << vert.z << ")\n";
  }

  // TextCord
  std::cout << "\nTexture Cordination:\n";
  for (const auto& textCord : m_textCord)
  {
    std::cout << "(" << textCord.u << " " << textCord.v << ")\n";
  }

  // Normal
  std::cout << "\nVector Normal:\n";
  for (const auto& normalVert : m_normalVertices)
  {
    std::cout << "(" << normalVert.x << " " << normalVert.y << " " << normalVert.z << ")\n";
  }

  for (const auto& face : m_faces)
  {
    for (const auto& vert : face)
    {
      std::cout << "(" << vert.m_geoId << " " << vert.m_texId << " " << vert.m_norId << ")\n";
    }
  }
}

std::vector<Vertex> ObjImporter::getGeometry()
{
    std::vector<Vertex> m_geometry;


    // for all faces
    for (uint64_t currentIndices = 0; const auto& face : m_faces)
    {
        for (const auto& vertex: face)
        {
            m_geometry.emplace_back(
                Vertex(
                m_geoVertices.at(vertex.m_geoId),
                m_textCord.at(vertex.m_texId),
                { 1.0f, 0.0f, 0.0f },
                m_normalVertices.at(vertex.m_norId))
            );
        }
        

        if (face.size() == 4) //When face containe 4 vertices 
        {
            //Triangle orientation
            //For ccw = 0  cc = 1
            uint64_t offset = 0;

            //First triangle
            m_indices.emplace_back(currentIndices);
            m_indices.emplace_back(currentIndices + 1 + offset);
            m_indices.emplace_back(currentIndices + 2 - offset);

            //Second triangle
            m_indices.emplace_back(currentIndices);
            m_indices.emplace_back(currentIndices + 2 + offset);
            m_indices.emplace_back(currentIndices + 3 - offset);

            currentIndices += 4;
        }

    }

  return m_geometry;
}

std::vector<uint32_t> st::io::ObjImporter::getIndicesVector() const
{
  return m_indices;
}

constexpr void ObjImporter::parseVertexLine(const std::string_view& vertexLine)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

}

}
