#ifndef VIEWPORT_MODELSMENAGER_HPP
#define VIEWPORT_MODELSMENAGER_HPP

#include <Geometry/Vector3.hpp>

namespace st::viewport
{
	class Model
	{

	};
	

	class Sphere
	{

		public:
		Sphere(geometry::Vector3 position, float radius, uint32_t subdivisionWidth, uint32_t subdivisionHeight);


		geometry::Vector3 position;
		
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
	};

};

#endif // VIEWPORT_MODELSMENAGER_HPP
