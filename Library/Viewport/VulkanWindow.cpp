#include "VulkanWindow.hpp"

#include <QMouseEvent>
#include <QKeyEvent>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <span>

#include <Geometry/Primitives/SphereMesh.hpp>
#include <Geometry/Primitives/PlaneMesh.hpp>

#include <Geometry/Body/StaticBody.hpp>
#include <Geometry/Body/SimulatedBody.hpp>
#include <Geometry/Model.hpp>
#include "Math/Math.hpp"

namespace st::viewport
{

	VulkanWindow::VulkanWindow():
		m_instance(std::make_unique<renderer::StInstance>()),
		m_surface(),
		m_renderer(),
		m_scene(),
		m_modelMenager(),
		m_simulationEngine(m_modelMenager)
	{
		setSurfaceType(QSurface::VulkanSurface);
	}


	void VulkanWindow::initialize()
	{

		m_scene.initialize();
		m_simulationEngine.initialize();

		m_instance->create();
		createQtInstance(m_instance->getInstance());
		auto surface = static_cast<vk::SurfaceKHR>(QVulkanInstance::surfaceForWindow(this));
		if (!surface)
		{
			exit(999);
		}


		m_surface = std::make_unique<renderer::Surface>(surface);
		m_renderer = std::make_unique<renderer::Renderer>(*m_instance, *m_surface, m_modelMenager);


		m_renderer->updateSwapChain(static_cast<uint64_t>(this->size().width()), static_cast<uint64_t>(this->size().height()));
		m_renderer->initialize();


		isInitialised = true;


		//Model Menager
		// Model Menager addModel (GenerateSphere)
		//// add spehere, static
		//Load Sphere
		//Load Plane (cloth)
		//First Mesh
		//io::ImporterProxy importerProxy;
		//importerProxy.readFile("../Assets/Models/Cube.obj");


		//geometry::SphereMesh sphere { 1.0f, 10u, 10u };

		std::unique_ptr<geometry::ShapeBase> sphere = std::make_unique<geometry::SphereMesh>(1.0f, 10u, 10u);
		std::unique_ptr<geometry::BodyBase> sphereBody =
		std::make_unique<geometry::StaticBody>(math::Vector3 { 0.0F, 0.0F, 0.0F }, math::Vector3 { 0.0f, 0.0f, 0.0 });


		
		math::Vector3 { 0.0f, 0.0f, 0.0f };
		
		int texWidth = 0;
		int texHeight = 0;
		int texChannels = 0;
		
		stbi_uc* pixels = stbi_load("../Assets/Textures/texture2.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);


		std::span<std::byte> pixelsByte { reinterpret_cast<std::byte*>(pixels), static_cast<std::span<std::byte>::size_type>(texWidth * texHeight * 4) };
		
		geometry::Texture texture { texWidth, texHeight, texChannels, pixelsByte };
		
		std::unique_ptr<geometry::Material> material = std::make_unique<geometry::Material>(texture);

		geometry::Model firstModel(std::move(sphere), std::move(sphereBody), std::move(material));
		m_modelMenager.addModel(std::move(firstModel));


		//-------------------------------------------------------------------Second Mesh-------------------------------------------------------------------------
		//io::ImporterProxy importerProxy2;
		//importerProxy2.readFile("../Assets/Models/Sphere.obj");

		//Plane plane {
		//	Vector3 {0.0f, 2.0f, 0.0f},
		//	3.0f,
		//	3.0f,
		//	59,
		//	59
		//};
		//
		stbi_uc* pixels2 = stbi_load("../Assets/Textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

		std::span<std::byte> pixelsByte2 { reinterpret_cast<std::byte*>(pixels2), static_cast<std::span<std::byte>::size_type>(texWidth * texHeight * 4) };
		
		geometry::Texture texture2 { texWidth, texHeight, texChannels, pixelsByte2 };
		std::unique_ptr<geometry::Material> material2 = std::make_unique<geometry::Material>(texture2);


		std::unique_ptr<geometry::PlaneMesh> plane = std::make_unique<geometry::PlaneMesh>(3.0f,3.0F,  59u, 59u);
		
		for (auto& vertex : plane->m_vertices)
		{
			vertex.m_pos = vertex.m_pos + math::Vector3 { 0.0F, 2.0F, 0.0F };
		}


		std::unique_ptr<geometry::BodyBase> sphereBody2 =
			std::make_unique<geometry::SimulatedBody>(math::Vector3 { 0.0F, 0.0F, 0.0F }, math::Vector3 { 0.0f, 0.0f, 0.0 });



		geometry::Model secondModel(std::move(plane), std::move(sphereBody2), std::move(material2));
		m_modelMenager.addModel(std::move(secondModel));
		m_renderer->updateRecourses();
	}


	void VulkanWindow::createQtInstance(vk::Instance instance)
	{
		inst.setVkInstance(instance);

		if (!inst.create())
		{
			qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
		}

		setVulkanInstance(&inst);
	}

	void VulkanWindow::resizeEvent(QResizeEvent* event)
	{
		if (isInitialised)
		{
			recreateSwapChain();
		}
		QWindow::resizeEvent(event);
	}

	bool VulkanWindow::event(QEvent* event)
	{
		switch (event->type())
		{
		case QEvent::UpdateRequest:
			update();
			break;

		case QEvent::PlatformSurface:
			if (static_cast<QPlatformSurfaceEvent*>(event)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed)
			{
				m_renderer->releaseResources();
			}
			break;

		default:
			break;
		}

		return QWindow::event(event);
	}

	void VulkanWindow::mousePressEvent(QMouseEvent* event)
	{
		// left alt + left   mouse click + move -> rotate
		// left alt + middle mouse click + move -> pan
		// left alt + right  mouse click + move up and down -> zoom
		auto m_fromClick = event->position().toPoint();

		//record start and stop
		if (event->modifiers() & Qt::AltModifier)
		{
			if (event->buttons() & Qt::LeftButton)
			{
				m_renderer->mousePressEvent(m_fromClick.x(), m_fromClick.y(), renderer::Camera::Actions::Orbit);
			}

			if (event->buttons() & Qt::MiddleButton)
			{
				qDebug() << "Middle (Pan) Start:" << m_fromClick << "\n";

				m_renderer->mousePressEvent(m_fromClick.x(), m_fromClick.y(), renderer::Camera::Actions::Pan);
			}

			if (event->buttons() & Qt::RightButton)
			{
				qDebug() << "Right (Zoom) Start:" << m_fromClick << "\n";

				m_renderer->mousePressEvent(m_fromClick.x(), m_fromClick.y(), renderer::Camera::Actions::Zoom);
			}
		}

		event->accept();
	}

	void VulkanWindow::mouseMoveEvent(QMouseEvent* event)
	{
		const auto mousePosition = event->position().toPoint();
		m_renderer->mouseMoveEvent(mousePosition.x(), mousePosition.y());
	}

	void VulkanWindow::mouseReleaseEvent(QMouseEvent* event)
	{
		const auto mousePosition = event->position().toPoint();
		m_renderer->mouseReleaseEvent(mousePosition.x(), mousePosition.y());
	}

	void VulkanWindow::keyPressEvent(QKeyEvent* event) { }

	void VulkanWindow::keyReleaseEvent(QKeyEvent* event) { }

	void VulkanWindow::exposeEvent(QExposeEvent* /*unused*/)
	{
		if (isExposed())
		{
			requestUpdate();
		}
	}

	void VulkanWindow::recreateSwapChain()
	{
		if (isInitialised)
		{
			m_renderer->recreateSwapChain(static_cast<uint64_t>(this->size().width()), static_cast<uint64_t>(this->size().height()));
		}
	}

	void VulkanWindow::update()
	{
		static auto previousTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		deltaTime = elapsedTime.count();

		//simulation
		m_simulationEngine.update(deltaTime);

		//drawFrame();
		m_renderer->renderFrame();
		requestUpdate();
	}

	void VulkanWindow::resetPlanePosition()
	{
		geometry::Mesh* mesh = dynamic_cast<geometry::Mesh*>(m_modelMenager.getModelsToSimulate().at(1)->m_shape.get());

		for (auto& vertex : mesh->m_vertices)
		{
			vertex.m_pos = vertex.m_pos + math::Vector3 { 0.0F, 2.0F, 0.0F };
		}
	}

} // namespace st::viewport
