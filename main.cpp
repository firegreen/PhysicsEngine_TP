#include <QApplication>
#include <QMainWindow>
#include <QtGui>
#include <QtOpenGL/QGL>

#include "glwidget.h"
#include "particule.h"
#include "edge.h"

int main(int argc, char *argv[])
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QApplication a(argc, argv);
    QMainWindow* window = new QMainWindow();
    GLWidget* widget = new GLWidget(window, "Physics Engine");
    window->setCentralWidget(widget);
    window->resize(500,500);
    window->show();

    Edge e1(QPointF(-300,-200), QPointF(300,-200));
    Edge e2(QPointF(-300,400), QPointF(-200,-200));
    Edge e3(QPointF(200,-200), QPointF(300,400));
    Edge e4(QPointF(300,400), QPointF(-300,400));

    widget->addActor(e1);
    widget->addActor(e2);
    widget->addActor(e3);
    widget->addActor(e4);

    for (int i=0; i < 20; ++i)
    {
        widget->addRandomParticule();
    }

    widget->setFocus();

    return a.exec();
}
