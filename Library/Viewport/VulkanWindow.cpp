#include "VulkanWindow.hpp"

#include <QMouseEvent>
#include <QKeyEvent>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <span>


namespace st::viewport
{

	VulkanWindow::VulkanWindow(): m_instance(std::make_unique<renderer::StInstance>()), m_surface(), m_renderer()
	{
		setSurfaceType(QSurface::VulkanSurface);
	}

	void VulkanWindow::initialize()
	{
		m_simulationEngine.initialize();

		m_instance->create();
		createQtInstance(m_instance->getInstance());
		auto surface = static_cast<vk::SurfaceKHR>(QVulkanInstance::surfaceForWindow(this));
		if (!surface)
		{
			exit(999);
		}


		m_surface = std::make_unique<renderer::Surface>(surface);
		m_renderer = std::make_unique<renderer::Renderer>(*m_instance, *m_surface);


		m_renderer->updateSwapChain(static_cast<uint64_t>(this->size().width()), static_cast<uint64_t>(this->size().height()));
		m_renderer->initialize();


		isInitialised = true;



		//Model Menager
		// Model Menager addModel (GenerateSphere)
		//// add spehere, static
		//Load Sphere
		//Load Plane (cloth)

		io::ImporterProxy importerProxy;
		importerProxy.readFile("../Assets/Models/Cube.obj");


		int texWidth = 0;
		int texHeight = 0;
		int texChannels = 0;

		stbi_uc* pixels = stbi_load("../Assets/Textures/texture2.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);


		std::span<std::byte> pixelsByte { reinterpret_cast<std::byte*>(pixels), static_cast<std::span<std::byte>::size_type>(texWidth * texHeight * 4) };

		renderer::Texture texture { 
			texWidth,
			texHeight, 
			texChannels, 
			pixelsByte
		};

		renderer::Mesh mesh {
			importerProxy.getVertices(),
			importerProxy.getIndices(),
			texture
		};


		//
		m_renderer->addResources(mesh);

	}

	void VulkanWindow::releaseResources()
	{
		m_renderer->releaseResources();
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
			// When windows is created or deleted
			// d->releaseSwapChain();
			//d->reset();
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


} // namespace st::viewport
