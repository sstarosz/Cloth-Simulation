#ifndef IO_OBJIMPORTER_HPP
#define IO_OBJIMPORTER_HPP

#include <filesystem>

namespace st::io {

struct Vec3
{
  float x;
  float y;
  float z;
};

struct Vec2
{
  float u;
  float v;
};

struct Vertex
{
  Vec3 pos;
  Vec2 textureCord;
  Vec3 color;
  Vec3 normal;
};

class ObjImporter
{
  public:
  ObjImporter();


  struct ObjImportOptions
  {
  };

  void readFromFile(const std::filesystem::path& pathToObjFile);

  void printData() const;

  std::vector<Vertex> getGeometry() const;

  std::vector<uint32_t> getIndicesVector() const;

  private:
  std::vector<Vec3> m_geoVertices;
  std::vector<Vec2> m_textCord;
  std::vector<Vec3> m_normalVertices;


  struct vertexId
  {
    uint32_t m_geoId;
    uint32_t m_texId;
    uint32_t m_norId;
  };

  using faceID = std::vector<vertexId>;


  std::vector<faceID> m_faces; //-> object
  std::vector<uint32_t> m_indices;
};

}


#endif // !IO_OBJIMPORTER_HPP
