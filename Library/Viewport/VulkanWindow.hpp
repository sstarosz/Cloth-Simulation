
#ifndef VULKANWINDOW_HPP
#define VULKANWINDOW_HPP


#include "Config.hpp"
#include "vulkan/vulkan.hpp"
#include <QWindow>
#include <QVulkanInstance>
#include <memory>


#include "Renderer/Instance.hpp"
#include "Renderer/Surface.hpp"
#include "Renderer/Renderer.hpp"

#include "Simulation/SimulationEngine.hpp"

namespace st::viewport
{
	class VulkanWindow : public QWindow
	{
		Q_OBJECT

	public:
		VulkanWindow();


		void initialize();
		void releaseResources();

	protected:
		//TO-DO - Change expose event as vulkan init
		void exposeEvent(QExposeEvent*) override;
		void resizeEvent(QResizeEvent*) override;
		bool event(QEvent* ev) override;

	private slots:
		void update();

	private:
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* eventvoverride);
		void mouseMoveEvent(QMouseEvent* event) override;
		void keyPressEvent(QKeyEvent* event) override;
		void keyReleaseEvent(QKeyEvent* event) override;


		QVulkanInstance inst;
		std::unique_ptr<renderer::StInstance> m_instance;
		std::unique_ptr<renderer::Surface> m_surface;
		std::unique_ptr<renderer::Renderer> m_renderer;


		simulation::SimulationEngine m_simulationEngine;


		  float deltaTime = 0.0f;

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		bool isInitialised = false;

		/*Init*/
		void createQtInstance(vk::Instance instance);
		void recreateSwapChain();
	};

} //!namespace st::viewport
#endif // !VULKANWINDOW_HPP
