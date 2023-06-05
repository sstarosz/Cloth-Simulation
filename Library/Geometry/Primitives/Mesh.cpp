#include "Mesh.hpp"


namespace st::geometry
{
	Mesh::Mesh()
	{

	}

	ShapeBase::ShapeType Mesh::getType() const
	{
		return ShapeType::eMesh;
	}

}