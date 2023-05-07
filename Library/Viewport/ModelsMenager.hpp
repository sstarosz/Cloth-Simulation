#ifndef VIEWPORT_MODELSMENAGER_HPP
#define VIEWPORT_MODELSMENAGER_HPP

#include <Geometry/Vector3.hpp>
#include <Geometry/Vertex.hpp>

#include <vector>
#include <memory>

namespace st::viewport
{
	using namespace geometry;



	struct Texture
	{


		//TODO clean buffer at the end
		uint32_t textureWidth;
		uint32_t textureHeight;
		uint32_t texChannels;

		std::span<std::byte> pixels;
	};

	
	struct Mesh
	{
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};


	class Model
	{
	public:
		Model() = default;
		Model(std::vector<Vertex> geometry, std::vector<uint32_t> indices) noexcept:
			m_mesh { geometry, indices },
			m_texture {} {};

		Model(Mesh mesh, Texture texture):
			m_mesh(mesh),
			m_texture(texture)
		{

		}

		Mesh m_mesh;
		Texture m_texture;
	};


	class ModelMetaData
	{ };


	class Sphere : public Mesh
	{

	public:
		Sphere(Vector3 position, float radius, uint32_t subdivisionWidth, uint32_t subdivisionHeight);


		//const Model& model;
	};


	class Plane : public Mesh
	{

	public:
		explicit Plane(Vector3 position, float width, float height, uint32_t subdivisionWidth, uint32_t subdivisionHeight);


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
		void addModel(Model&& model);
		
		//Add Geometry that contain only mesh data
		void addGeometry();

		void addResource() {};

		const std::vector<Model>& getModelsToRender() const
		{
			return m_models;
		}

		std::vector<Model>& getModelsToSimulate()
		{
			return m_models;
		}

	private:
		// List of all models

		std::vector<Model> m_models;
	};



	//one class
	//array of vertixes 

	//second class reference to element in vector and meta-data
};

#endif // VIEWPORT_MODELSMENAGER_HPP
