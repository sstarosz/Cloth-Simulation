#include <QApplication>

#include <MainWindow/MainWindows.hpp>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
