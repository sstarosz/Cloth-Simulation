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
#include <QVBoxLayout>
#include <QAction>

#include <Viewport/ViewportWidget.hpp>


namespace st
{

MainWindow::MainWindow()
{
    //Setup GUI
    //Setup ToolBar
    //Setup StatusBar
    //Setup Main Widget/Viewport 3D
    //Dock Widget
	this->setFixedSize(800, 600);
    
    createMenuBar();
    createStatusBar();
    createMainWindow();
}

void MainWindow::setupToolBar()
{
    /*fileMenu = menuBar()->addMenu("&File");
    auto* quit = new QAction("&Quit", this);
    fileMenu->addAction(quit);*/


    //QVBoxLayout* mainlayout = new QVBoxLayout(this);
    //navigationBar = new NavigationBar(this);
    //navigationBar->setLayout(mainlayout);



}
void MainWindow::createMenuBar()
{

    m_fileMenu = menuBar()->addMenu("&File");
    auto* newScene = new QAction("&New Scene", this);
    auto* quit = new QAction("&Quit", this);

    m_fileMenu->addAction(newScene);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(quit);

    m_fileMenu->setTearOffEnabled(true);


    m_helpMenu = menuBar()->addMenu("&Help");
    auto* about = new QAction("&About", this);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Text"));
}

void MainWindow::createMainWindow()
{
    //QWidget* centralWidget = new QWidget();

    viewportWidget = new viewport::ViewportWidget();
    setCentralWidget(viewportWidget);
}

}