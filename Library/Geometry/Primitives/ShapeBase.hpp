#ifndef GEOMETRY_PRIMITIVES_SHAPEBASE_HPP
#define GEOMETRY_PRIMITIVES_SHAPEBASE_HPP


namespace st::geometry
{

	class ShapeBase
	{

	protected:
		enum class ShapeType
		{
			eSphereShape,
			eMesh
		};

	public:

		virtual ~ShapeBase() = default;

		virtual ShapeType getType() const = 0;
	};


};

#endif 
