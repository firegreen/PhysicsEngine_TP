#include <QApplication>
#include <QMainWindow>
#include <QtGui>
#include <QtOpenGL/QGL>

#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow* window = new QMainWindow();
    GLWidget* widget = new GLWidget(window, "Physics Engine");
    window->setCentralWidget(widget);
    window->resize(500,500);
    window->show();

    Particle p1 = Particle(-100,50, 5);
    Particle p2 = Particle(-100,50, 5, Qt::red);
    Particle p3 = Particle(-100,50, 5, Qt::blue);

    p2.updateType = Actor::Implicit;
    p3.updateType = Actor::LeapFrog;

    widget->addParticule(p1);
    widget->addParticule(p2);
    widget->addParticule(p3);

    return a.exec();
}
