#ifndef MAINWINDOWS_H
#define MAINWIDNOWS_H

#include <QMainWindow>

class QToolBar;
class NavigationBar;

class MainWindow : public QMainWindow
{
public:
	MainWindow();
	~MainWindow();

private:
	QToolBar* toolBar;
	NavigationBar* navigationBar;

	void setupToolBar();
};


#endif // !MAINWINDOWS_H
