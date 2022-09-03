#ifndef MAINWINDOW_H
#define MAINWIDNOW_H

#include <QMainWindow>


class QToolBar;
class NavigationBar;
class QMenu;


namespace st
{

namespace viewport
{
	class ViewportWidget;
}


class MainWindow : public QMainWindow
{
public:
	MainWindow();

private:
	QToolBar* toolBar;
	NavigationBar* navigationBar;
	viewport::ViewportWidget* viewportWidget;

	QMenu* m_fileMenu;
	QMenu* m_helpMenu;

	void setupToolBar();

	void createMenuBar();
	void createStatusBar();
	void createMainWindow();
};

}
#endif // !MAINWINDOW_H
