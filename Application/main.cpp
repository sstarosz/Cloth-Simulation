#include <QApplication>

//#include <MainWindows.hpp>
//#include <Viewport/MainWindows.hpp>
#include <MainWindow/MainWindows.hpp>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
