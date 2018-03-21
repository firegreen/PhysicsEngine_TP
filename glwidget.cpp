#include <QEvent>
#include <iostream>

#include "glwidget.h"
#include "GL/glu.h"
#include "link.h"
#include "edge.h"

GLWidget::GLWidget(QWidget *parent, int framesPerSecond)
    : QGLWidget(parent), gravity(0,-91), stop(false), debug(false), _lock(false),
      linkWithWall(false), linkedParticules(false), wallLink1(nullptr), wallLink2(nullptr)
{
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

GLWidget::~GLWidget()
{
    resetMovableActors();
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

    lock();
    if (wallLink1)
        wallLink1->draw(debug);
    if (wallLink2)
        wallLink2->draw(debug);
    for(const QSharedPointer<Actor>& a : actors)
        a->draw(debug);
    for(const QSharedPointer<Link>& s : links)
        s->draw(debug);
    unlock();
}

void GLWidget::update()
{
    if (!stop)
    {
        if (!elapsedTime.isValid())
            elapsedTime.restart();
        const float elapsedS = timeCoef * (float) elapsedTime.restart()  / 1000.f;

        lock();

        if (wallLink1)
            wallLink1->update(elapsedS);
        if (wallLink2)
            wallLink2->update(elapsedS);
        for(QMutableListIterator<QSharedPointer<Link>> i(links); i.hasNext();)
        {
            if (!i.next()->update(elapsedS))
                i.remove();
        }

        for(QMutableListIterator<QSharedPointer<Actor>> i(actors); i.hasNext();)
        {
            if (!i.next()->update(elapsedS))
                i.remove();
        }
        unlock();

        ++Actor::frame;
    }
    else
        elapsedTime.invalidate();
    updateGL();
}

class SleeperThread : public QThread
{
    public:
    static void usleep(unsigned long msecs)
    {
        QThread::usleep(msecs);
    }
};

void GLWidget::lock()
{
    while (_lock)
        SleeperThread::usleep(60);
    _lock = true;
}

void GLWidget::unlock()
{
    _lock = false;
}

void GLWidget::setGlue(float value)
{
    for(QSharedPointer<Actor>& a : actors)
    {
        if (MovableActor* ma = dynamic_cast<MovableActor*>(a.data()))
        {
            ma->glueCoef = value;
        }
    }
}

void GLWidget::setElasticity(float value)
{
    for(QSharedPointer<Actor>& a : actors)
    {
        if (MovableActor* ma = dynamic_cast<MovableActor*>(a.data()))
        {
            ma->elasticity = value;
        }
    }
}

void GLWidget::resetMovableActors(int newParticulesCount)
{
    lock();
    if (wallLink1)
    {
        delete wallLink1;
        wallLink1 = nullptr;
    }
    if (wallLink2)
    {
        delete wallLink2;
        wallLink2 = nullptr;
    }
    for(QMutableListIterator<QSharedPointer<Actor>> i(actors); i.hasNext();)
    {
        if (dynamic_cast<MovableActor*>(i.next().data()))
        {
            i.remove();
        }
    }
    for (int i=0; i < newParticulesCount; ++i)
    {
        addRandomParticule();
    }
    unlock();
}

Particle& GLWidget::addRandomParticule()
{
    QColor c(qrand() % 170+50, qrand() % 200+50, qrand() % 40 + 70);
    Particle* p = new Particle(qrand() % 400-250,qrand() % 120+250, qrand() % 10 + 3, c);
    p->addForce(QVector2D(qrand() % 100 - 50,qrand() % 10 - 5));
    Particle& newP = addParticule(*p);

    if (linkWithWall)
    {
        if (!wallLink1)
        {
            for(QSharedPointer<Actor>& a : actors)
            {
                Edge* e = dynamic_cast<Edge*>(a.data());
                if (e && qrand() % 3 == 0)
                {
                    wallLink1 = new HardLink(actors.last(), a, p->getPosition(),
                                           e->getLine().pointAt(std::min(1.0f, (float)(qrand()%100)*0.01f)),
                                            0.2f+(float)(qrand()%70)*0.05, 10, 30+(float)(qrand()%200)*0.05f);
                    break;
                }
            }
        }

        if (wallLink2)
        {
            delete wallLink2;
            wallLink2 = nullptr;
        }
        for(QSharedPointer<Actor>& a : actors)
        {
            Edge* e = dynamic_cast<Edge*>(a.data());
            if (e && qrand() % 3 == 0)
            {
                wallLink2 = new HardLink(actors.last(), a, p->getPosition(),
                                         e->getLine().pointAt(std::min(1.0f, (float)(qrand()%100)*0.01f)),
                                         0.2f+(float)(qrand()%70)*0.05f, 10, 35+(float)(qrand()%200)*0.05f);
                break;
            }
        }
    }

    if (linkedParticules && actors.length()>1)
    {
        QList<QSharedPointer<Actor> >::iterator last = actors.end();
        last--; last--;
        if (MovableActor* ma = dynamic_cast<MovableActor*>(last->data()))
        {
            HardLink* link = new HardLink(actors.last(), *last, p->getPosition(),
                                          ma->getPosition(),0.7f+(float)(qrand()%30)*0.05f,10,35+(float)(qrand()%100)*0.05f);
            addLink(*link);
        }
    }

    return newP;
}

Particle& GLWidget::addParticule(Particle &p)
{
    return reinterpret_cast<Particle&>(addActor(p));
}

Actor &GLWidget::addActor(Actor &p)
{
    QSharedPointer<Actor> ptr(&p);
    for(QList<QSharedPointer<Actor>>::iterator i = actors.begin(); i != actors.end(); ++i)
    {
        Link* s = new Link(ptr, *i, 1.5,0,0.7,false,true);
        addLink(*s);
    }

    actors.append(ptr);
    Actor* last = actors.last().data();
    last->addConstantAcceleration(gravity);
    return *last;
}

Link &GLWidget::addLink(Link &s)
{
    links.append(QSharedPointer<Link>(&s));
    return *links.last();
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
