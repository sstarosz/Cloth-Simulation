#include "ViewportWidget.hpp"

#include <QTimer>
#include <QVBoxLayout>

namespace st::viewport
{

	ViewportWidget::ViewportWidget()
	{

		auto* viewportLayout = new QVBoxLayout();

		/* Viewport Layout*/
		vulkan_window = new VulkanWindow;
		auto* vulkanContainer = QWidget::createWindowContainer(vulkan_window);
		vulkanContainer->setFocusPolicy(Qt::StrongFocus);
		vulkanContainer->setFocus();
		//vulkanContainer->setMinimumSize(800, 800);
		//vulkanContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		viewportLayout->addWidget(vulkanContainer);

		setLayout(viewportLayout);

		QTimer::singleShot(2, this, &ViewportWidget::init);
	}

	void ViewportWidget::init()
	{
		vulkan_window->initialize();
	}


} // namespace st::viewport
