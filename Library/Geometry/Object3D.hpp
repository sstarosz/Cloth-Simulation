#ifndef GEOMETRY_OBJECT3D_HPP
#define GEOMETRY_OBJECT3D_HPP

#include <vector>
#include "Vertex.hpp"

namespace st::geometry
{

class Object3D
{
  std::vector<Vertex> m_vertices;
  std::vector<uint64_t> m_indices;
};


}

#endif // !GEOMETRY_OBJECT3D_HPP
