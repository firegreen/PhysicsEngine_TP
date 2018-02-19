#include <QLineF>

#include "edge.h"
#include "glwidget.h"
#include "movableactor.h"
#include "GL/glu.h"

Edge::Edge(const QPointF &p1, const QPointF &p2, const QVector2D &relativeForce)
    : line(p1, p2), relativeForce(relativeForce)
{    
    QLineF normal = line.normalVector().unitVector();
    QVector2D vector(normal.dx(), normal.dy());
    this->relativeForce *= vector;
}

void Edge::draw(bool debug) const
{
    glBegin(GL_LINE);
    glVertex2f(line.p1().x(), line.p1().y());
    glVertex2f(line.p2().x(), line.p2().y());
    glEnd();
}

void Edge::collision(Actor &other) const
{
    MovableActor* mActor = dynamic_cast<MovableActor*>(&other);
    if (mActor)
        other.addForce(relativeForce * mActor->getSpeed());
}
