#ifndef GEOMETRY_PRIMITIVES_SHAPEBASE_HPP
#define GEOMETRY_PRIMITIVES_SHAPEBASE_HPP


namespace st::geometry
{

	class ShapeBase
	{

	public:
		enum class ShapeType
		{
			eSphereShape,
			eMesh
		};


		virtual ~ShapeBase() = default;

		virtual ShapeType getType() const = 0;
	};


};

#endif 
