#ifndef VIEWPORT_VIEWPORTWIDGET_HPP
#define VIEWPORT_VIEWPORTWIDGET_HPP

#include <QWidget>
#include <QCloseEvent>
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

#endif // VIEWPORT_VIEWPORTWIDGET_HPP
