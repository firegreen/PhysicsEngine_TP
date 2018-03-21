#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QTimer>
#include <QList>

#include "actor.h"
#include "particule.h"
#include "link.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:

    explicit GLWidget(QWidget *parent = 0, int framesPerSecond = 30);
    ~GLWidget();

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void keyPressEvent( QKeyEvent *keyEvent ) override;
    void mouseReleaseEvent( QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* event) override;

    Particle &addRandomParticule(QPointF pos = QPointF(), QVector2D force = QVector2D());
    Particle& addParticule(Particle& p);
    Actor& addActor(Actor &p);
    Link& addLink(Link &s);

    void tooglePause();
    void toogleDebug();

    static void drawEllipse(float x, float y, float radius, int res = 10);
    static void drawLine(float x1, float y1, float x2, float y2);

    float timeCoef = 1.0f;

    void setGlue(float value);
    void setElasticity(float value);
    void resetMovableActors(int newParticulesCount=0);

public slots:
    virtual void update();

public:
    bool linkedParticules;
    bool linkWithWall;
    bool stop;
    bool debug;

    bool emitter;
    QPointF emitterPos;
    QVector2D emitterForce;
    int emitterFreq;
    int emitterGenCount;

    float glueCoef;

private:
    void lock();
    void unlock();

    QTimer *t_Timer;
    QElapsedTimer elapsedTime;

    QVector2D gravity;

    bool _lock;

    QList<QSharedPointer<Actor>> actors;
    //QList<QSharedPointer<HardLink>> springs;
    HardLink* wallLink1;
    HardLink* wallLink2;
    QList<QSharedPointer<Link>> links;
};
#endif // GLWIDGET_H
