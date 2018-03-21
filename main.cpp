#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"
#include <QtGui>
#include <QtOpenGL/QGL>

int main(int argc, char *argv[])
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QApplication a(argc, argv);
    MainWindow* window = new MainWindow();
    window->show();

    return a.exec();
}
