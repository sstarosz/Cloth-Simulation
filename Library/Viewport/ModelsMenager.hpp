#ifndef VIEWPORT_MODELSMENAGER_HPP
#define VIEWPORT_MODELSMENAGER_HPP

#include <Geometry/Vector3.hpp>
#include <Geometry/Vertex.hpp>

#include <vector>
#include <memory>

namespace st::viewport
{
	using namespace geometry;


	class Model
	{
	public:
		Model() = default;
		std::vector<Vertex> m_geometry;
	};


	class ModelMetaData
	{ };


	class Sphere : public Model
	{

	public:
		Sphere(Vector3 position, float radius, uint32_t subdivisionWidth, uint32_t subdivisionHeight);


		//const Model& model;
	};


	class Plane : public Model
	{

	public:
		Plane(Vector3 position, uint32_t width, uint32_t height, uint32_t subdivisionWidth, uint32_t subdivisionHeight);


		//const Model& model;
	};


	enum class TextureType
	{
		eNone,
		eUseTexture
	};


	class RenderableObjectMetaData
	{
		//Descriptor Sets (UBO, ImageSampler (Texture))
		//Texture
		TextureType m_textureType;


		//
		//
		//pipline?
		//
		// IsDynamic -> MemoryType base on the purpose of the mesh
		//
		//
		// Vertexs Array
		//VertexBuffer -> will be created on renderer side
		//Index Buffer ->		-||-

		const Model& m_model;
	};




	class SimulateObjectMetaData
	{ };


	class ModelsMenager
	{

	public:
		ModelsMenager();


		//add model


		//get models for simulation
		//get models for rendering

		//High lever structure that can contain meshes, textures, etc (usually data that come's from 3d format files)
		void addModel(Model& model);
		
		//Add Geometry that contain only mesh data
		void addGeometry();

		void addResource() {};

		void getModelsToRender() const;

	private:
		// List of all models

		std::vector<std::unique_ptr<Model>> m_models;
	};

};

#endif // VIEWPORT_MODELSMENAGER_HPP
