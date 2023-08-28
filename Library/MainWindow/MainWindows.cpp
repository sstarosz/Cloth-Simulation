#include "MainWindows.hpp"

#include "NavigationBar.hpp"

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>

#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QToolBox>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QAction>

#include "Viewport/ViewportWidget.hpp"
#include "GUI/SettingsView.hpp"
#include "GUI/TransformationView.hpp"
#include "GUI/ToolBar.hpp"

namespace st
{

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags):
QMainWindow(parent, flags)
{
    setObjectName("MainWindow");

    //Setup GUI
    
    p_mSettingsView = new ui::gui::SettingsView();
	this->setMinimumSize(800, 600);
    
    createMenuBar();
    setupToolBar();
    createStatusBar();
    createDockWindows();
    createMainWindow();

    connect(this, &MainWindow::resetPlanePosition,
            p_mViewportWidget, &viewport::ViewportWidget::receiveCommandSlot);
}

void MainWindow::setupToolBar()
{
    for (int i = 0; i < 1; ++i)
	{
        std::unique_ptr<ui::gui::ToolBar> tb = std::make_unique<ui::gui::ToolBar>(QString::fromLatin1("Tool Bar %1").arg(i + 1), this);
		m_pToolBars.push_back(tb.get());
		addToolBar(tb.release());
	}

    connect(m_pToolBars.at(0), &ui::gui::ToolBar::resetPlanePosition,
            this, &MainWindow::receiveCommandForViewport);
}

void MainWindow::createMenuBar()
{

	m_fileMenu = menuBar()->addMenu("&File");
	std::unique_ptr<QAction> newScene = std::make_unique<QAction>("&New Scene", this);


    /*Settings*/
	std::unique_ptr<QAction> settings = std::make_unique<QAction>("&Settings", this);
    settings->setStatusTip("Open Settings Menu");
	connect(settings.get(), &QAction::triggered, this, &MainWindow::openSettingsView);


	std::unique_ptr<QAction> quit = std::make_unique<QAction>("&Quit", this);

    m_fileMenu->addAction(newScene.release());
    m_fileMenu->addAction(settings.release());
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(quit.release());

    m_fileMenu->setTearOffEnabled(true);


    m_helpMenu = menuBar()->addMenu("&Help");
	std::unique_ptr<QAction> about = std::make_unique<QAction>("&About", this);

	m_helpMenu->addAction(about.release());

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Text"));
}

void MainWindow::createMainWindow()
{
	std::unique_ptr<viewport::ViewportWidget> viewportWidget = std::make_unique<viewport::ViewportWidget>();
	p_mViewportWidget = viewportWidget.get();
    setCentralWidget(viewportWidget.release());
}

void MainWindow::createDockWindows()
{
	std::unique_ptr<QDockWidget> dock = std::make_unique<QDockWidget>(tr("Transformation"), this);

    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
 
	std::unique_ptr<ui::gui::TransformationView> transformationView = std::make_unique<ui::gui::TransformationView>();
	p_mTransformationView = transformationView.get();
	dock->setWidget(transformationView.release());

    addDockWidget(Qt::RightDockWidgetArea, dock.release());
}

void MainWindow::openSettingsView()
{
    p_mSettingsView->show();
}

void MainWindow::receiveCommandForViewport()
{
    emit resetPlanePosition();
}

}