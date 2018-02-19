#include <QEvent>

#include "glwidget.h"
#include "GL/glu.h"

GLWidget::GLWidget(QWidget *parent, char *name, int framesPerSecond)
    : QGLWidget(parent), gravity(0,-91)
{
    setWindowTitle(QString::fromUtf8(name));
    if(framesPerSecond == 0)
        t_Timer = NULL;
    else
    {
        int timerInterval = 1000 / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(update()));
        t_Timer->start( timerInterval );
    }
    elapsedTime.start();
}

//const GLfloat twicePi = 2.0f * PI;

void GLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-width/2, width/2, -height/2,height/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
        for (QSharedPointer<Actor>& p : actors)
            p->addForce(QVector2D(50,200));
    QGLWidget::mouseReleaseEvent(mouseEvent);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    timeCoef += event->delta() / 50.f;
}

void GLWidget::paintGL()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.2,0.7,0.4);

    for(const QSharedPointer<Actor>& a : actors)
        a->draw(true);
}

void GLWidget::update()
{
    float elapsedS = (float) elapsedTime.restart()  / 1000.f;

    // process forces
    /*for(Particule& p : particules)
    {
        p.addForce(gravity * elapsedS);
    }*/

    // update position
    for (QSharedPointer<Actor>& a : actors)
        a->update(elapsedS * timeCoef);

    ++Actor::frame;
    updateGL();
}

Particle& GLWidget::addParticule(Particle &p)
{
    actors.append(QSharedPointer<Actor>(&p));
    Particle* last = reinterpret_cast<Particle*>(actors.last().data());
    last->addConstantAcceleration(gravity);
    return *last;
}

Actor &GLWidget::addActor(Actor &p)
{
    actors.append(QSharedPointer<Actor>(&p));
    Actor* last = actors.last().data();
    last->addConstantAcceleration(gravity);
    return *last;
}

void GLWidget::drawEllipse(float x, float y, float radius, int res)
{
    float resolution = res;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    float step = (2.f*M_PI / resolution);
    for (float i = 0; i <= resolution; i++) {
        glVertex2f(
                    x + (radius * cos(i * step)),
                    y + (radius * sin(i * step))
                    );
    }
    glEnd();
}

void GLWidget::drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}
