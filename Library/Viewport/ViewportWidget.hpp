#ifndef VIEWPORTWIDGET_HPP
#define VIEWPORTWIDGET_HPP

#include <QWidget>

#include "VulkanWindow.hpp"


namespace st::viewport
{
	class ViewportWidget : public QWidget
	{
		Q_OBJECT

	public:
		ViewportWidget();

		void init();

	private:

		VulkanWindow* vulkan_window;

	};

};

#endif // VIEWPORTWIDGET_HPP
