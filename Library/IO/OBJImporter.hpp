#ifndef IO_OBJIMPORTER_HPP
#define IO_OBJIMPORTER_HPP

#include <filesystem>
#include <string_view>

namespace st::io {

struct Vec3
{
  float x;
  float y;
  float z;

  auto operator<=>(const Vec3&) const = default;
};

struct Vec2
{
  float u;
  float v;

  auto operator<=>(const Vec2&) const = default;
};

struct Vertex
{
  Vec3 pos;
  Vec2 textureCord;
  Vec3 color;
  Vec3 normal;

  auto operator<=>(const Vertex&) const = default;
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


  void parseObjFile();

  constexpr void parseVertexLine(const std::string_view& vertexLine);
  constexpr void parseTextureCordinateLine();
  constexpr void parseVertexNormalLine();



  struct vertexId
  {
    uint32_t m_geoId;
    uint32_t m_texId;
    uint32_t m_norId;
  };

  using faceID = std::vector<vertexId>;


  std::vector<faceID> m_faces; //-> object
  std::vector<uint32_t> m_indices;
  std::vector<Vertex> m_geometry;

};

}


#endif // !IO_OBJIMPORTER_HPP
