#include "MainWindows.hpp"
#include <QtWidgets/QToolBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QToolBox>
#include <QAction>

#include "NavigationBar.hpp"

MainWindow::MainWindow()
{
	this->setFixedSize(800, 600);
    
    setupToolBar();

}

MainWindow::~MainWindow()
{
}

void MainWindow::setupToolBar()
{

    toolBar = new QToolBar(this);
    toolBar->setMovable(false);
    toolBar->setAllowedAreas(Qt::LeftToolBarArea);
    toolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->setIconSize(QSize(50, 50));

    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->setOrientation(Qt::Orientation::Vertical);
    toolBar->setStyleSheet("background-color: darkgray");


    navigationBar = new NavigationBar(this);
    

    toolBar->addWidget(navigationBar);
    toolBar->addAction("Bookmark 1");
    toolBar->addAction("Bookmark 2");
    //toolBar->addWidget(separator);
    toolBar->addAction("More Bookmarks");


    addToolBar(Qt::LeftToolBarArea, toolBar);
}
