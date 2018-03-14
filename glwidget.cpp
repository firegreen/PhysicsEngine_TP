#include <QEvent>
#include <iostream>

#include "glwidget.h"
#include "GL/glu.h"
#include "spring.h"
#include "edge.h"

GLWidget::GLWidget(QWidget *parent, std::string name, int framesPerSecond)
    : QGLWidget(parent), gravity(0,-91), stop(false), debug(false)
{
    setWindowTitle(QString::fromStdString(name));
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
    case Qt::Key_D:
        toogleDebug();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Plus:
        gravity.setY(gravity.y() + 2);
        break;
    case Qt::Key_Minus:
        gravity.setY(gravity.y() - 2);
        break;
    case Qt::Key_Space:
        tooglePause();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        addRandomParticule();
        break;
    }
    QGLWidget::keyPressEvent(keyEvent);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
        for (QSharedPointer<Actor>& p : actors)
            p->addForce(QVector2D(qrand() % 100 - 50, qrand() % 200 - 50));
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
        a->draw(debug);
    for(const QSharedPointer<Spring>& s : springs)
        s->draw(debug);
}

void GLWidget::update()
{
    if (!stop)
    {
        if (!elapsedTime.isValid())
            elapsedTime.restart();
        const float elapsedS = timeCoef * (float) elapsedTime.restart()  / 1000.f;

        // process forces
        Intersection intersection;
        for(QList<QSharedPointer<Actor>>::iterator i = actors.begin(); i != actors.end(); ++i)
        {
            for (QList<QSharedPointer<Actor>>::iterator j = i+1; j != actors.end(); ++j)
            {
                if ((*i)->checkCollision(*(*j), intersection))
                {
                    //other.addForce(normal * abs(QVector2D::dotProduct(mActor->getNextSpeed(), normal)) * elasticity);
                    //Spring* spring = new Spring(*(*j), *(*i));
                    (*i)->collision(*(*j), intersection.normal2);
                    (*j)->collision(*(*i), intersection.normal1);
                }
            }
        }

        for(QMutableListIterator<QSharedPointer<Actor>> i(actors); i.hasNext();)
        {
            if (!i.next()->update(elapsedS))
                i.remove();
        }

        for(QMutableListIterator<QSharedPointer<Spring>> i(springs); i.hasNext();)
        {
            if (!i.next()->update(elapsedS))
                i.remove();
        }

        ++Actor::frame;
    }
    else
        elapsedTime.invalidate();
    updateGL();
}



Particle& GLWidget::addRandomParticule()
{
    QColor c(qrand() % 170+50, qrand() % 200+50, qrand() % 40 + 70);
    Particle* p = new Particle(qrand() % 200-100,qrand() % 200-70, qrand() % 10 + 3, c);
    p->addForce(QVector2D(qrand() % 100 - 50,qrand() % 100 - 50));
    if (actors.length() && qrand() % 3 == 0)
    {
        Actor* a2 = actors.at(qrand() % actors.length()).data();
        MovableActor* ma2 = dynamic_cast<MovableActor*>(a2);
        if (ma2)
        {
            Spring* s = new Spring(*p, *a2, p->getPosition(), ma2->getPosition(),
                                   (float)(qrand()%10)*0.5f);
            addSpring(*s);
        }
        else
        {
            Edge* e2 = dynamic_cast<Edge*>(a2);
            if (e2)
            {
                Spring* s = new Spring(*p, *a2, p->getPosition(),
                                       e2->getLine().pointAt(std::min(1.0f, (float)(qrand()%100)*0.01f)));
                addSpring(*s);
            }
        }

    }
    return addParticule(*p);
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

Spring &GLWidget::addSpring(Spring &s)
{
    springs.append(QSharedPointer<Spring>(&s));
    return *springs.last();
}

void GLWidget::tooglePause()
{
    stop = !stop;
}

void GLWidget::toogleDebug()
{
    debug = !debug;
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
