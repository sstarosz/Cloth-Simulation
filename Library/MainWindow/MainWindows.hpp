#ifndef MAINWINDOW_MAINWINDOW_HPP
#define MAINWINDOW_MAINWIDNOW_HPP

#include <QMainWindow>
#include <vector>

class QToolBar;
class NavigationBar;
class QMenu;


namespace st 
{

namespace viewport 
{
	class ViewportWidget;
}

namespace ui::gui
{
	class SettingsView;
	class TransformationView;
	class ToolBar;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT 
	
	public:
		explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});

	private:
		NavigationBar* navigationBar;
		viewport::ViewportWidget* p_mViewportWidget;
		ui::gui::SettingsView* p_mSettingsView;
		ui::gui::TransformationView* p_mTransformationView;

		std::vector<ui::gui::ToolBar*> m_pToolBars;

		QMenu* m_fileMenu;
		QMenu* m_helpMenu;

		void setupToolBar();

		void createMenuBar();
		void createStatusBar();
		void createDockWindows();
		void createMainWindow();

	signals:
        void resetPlanePosition();

	private slots:
		void openSettingsView();
		void receiveCommandForViewport();
};

}
#endif // !MAINWINDOW_MAINWINDOW_HPP
