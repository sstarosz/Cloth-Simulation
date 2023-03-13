#ifndef VIEWPORT_MODELSMENAGER_HPP
#define VIEWPORT_MODELSMENAGER_HPP

#include <Geometry/Vector3.hpp>
#include <Geometry/Vertex.hpp>

namespace st::viewport
{
	using namespace geometry;


	class Model
	{

		std::vector<Vector3> geometry;
	};

	
	class ModelMetaData
	{

	};



	class Sphere
	{

		public:
		Sphere(Vector3 position, float radius, uint32_t subdivisionWidth, uint32_t subdivisionHeight);


		//const Model& model;

		std::vector<Vertex> m_geometry;
	};







	class ModelsMenager
	{

	public:
		ModelsMenager();

		
		//add model


		//get models for simulation
		//get models for rendering

		void addModel(Model& model);

	private:
		// List of all models

		//Medata
	};

};

#endif // VIEWPORT_MODELSMENAGER_HPP
