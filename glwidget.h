#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QTimer>
#include <QList>

#include "actor.h"
#include "particule.h"
#include "spring.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:

    explicit GLWidget(QWidget *parent = 0, std::string name = 0, int framesPerSecond = 30);
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void keyPressEvent( QKeyEvent *keyEvent ) override;
    void mouseReleaseEvent( QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* event) override;

    Particle &addRandomParticule();
    Particle& addParticule(Particle& p);
    Actor& addActor(Actor &p);
    Spring& addSpring(Spring &s);

    void tooglePause();
    void toogleDebug();

    static void drawEllipse(float x, float y, float radius, int res = 10);
    static void drawLine(float x1, float y1, float x2, float y2);

    float timeCoef = 1.0f;

public slots:
    virtual void update();

private:
    QTimer *t_Timer;
    QElapsedTimer elapsedTime;

    QVector2D gravity;

    bool stop;
    bool debug;

    QList<QSharedPointer<Actor>> actors;
    QList<QSharedPointer<Spring>> springs;
};
#endif // GLWIDGET_H
