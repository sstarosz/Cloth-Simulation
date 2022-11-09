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

namespace ui::gui {
	class SettingsView;
}


class MainWindow : public QMainWindow
{
public:
	MainWindow();

private:
	QToolBar* toolBar;
	NavigationBar* navigationBar;
	viewport::ViewportWidget* viewportWidget;
	ui::gui::SettingsView* p_mSettingsView;

	QMenu* m_fileMenu;
	QMenu* m_helpMenu;

	void setupToolBar();

	void createMenuBar();
	void createStatusBar();
	void createMainWindow();


private slots:
	void openSettingsView();

};

}
#endif // !MAINWINDOW_H
