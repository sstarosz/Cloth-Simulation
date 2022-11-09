#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include <vector>
#include "Vertex.hpp"

namespace st::geometry {

class Object3D {
    std::vector<Vertex> m_vertices;
    std::vector<uint64_t> m_indices;

};



}

#endif // !OBJECT3D_HPP