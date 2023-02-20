#include "VulkanWindow.hpp"

#include "Line.hpp"
#include <QMouseEvent>
#include <QKeyEvent>


namespace st::viewport {

VulkanWindow::VulkanWindow():
	m_instance(std::make_unique<renderer::StInstance>()),
	m_surface(),
	m_renderer()
{
    setSurfaceType(QSurface::VulkanSurface);
}

void VulkanWindow::initialize()
{


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
    switch (event->type()) {
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
            m_renderer->mousePressEvent(m_fromClick.x(), m_fromClick.y(),  renderer::Camera::Actions::Orbit);

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

void VulkanWindow::keyPressEvent(QKeyEvent* event)
{

}

void VulkanWindow::keyReleaseEvent(QKeyEvent* event)
{

}

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
	    m_renderer->recreateSwapChain(
            static_cast<uint64_t>(this->size().width()),
			static_cast<uint64_t>(this->size().height())
		);
    }
}

void VulkanWindow::update()
{
    //simulate?
    
    //drawFrame();
	m_renderer->renderFrame();
	requestUpdate();
}





} // namespace st::viewport
