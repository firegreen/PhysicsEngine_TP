#include <QLineF>

#include "edge.h"
#include "glwidget.h"
#include "movableactor.h"
#include "GL/glu.h"

Edge::Edge(const QPointF &p1, const QPointF &p2, bool wall, const QColor &color,
           const float elasticity, const float viscosity)
    : p1(p1), p2(p2), color(color), elasticity(elasticity), viscosity(viscosity)
{
    QLineF vector(p1, p2);
    tangent = QVector2D(vector.dx(), vector.dy()).normalized();
    vector = vector.normalVector();
    normal = -QVector2D(vector.dx(), vector.dy()).normalized();
    if (wall)
        collider = QSharedPointer<Collider>(new WallCollider(this->p1, this->p2));
    else
        collider = QSharedPointer<Collider>(new LineCollider(this->p1, this->p2));
    hardness = 5.0f;
}

void Edge::draw(bool debug) const
{
    glColor3ub(color.red(), color.green(), color.blue());
    glBegin(GL_LINES);
    glVertex2f(p1.x(), p1.y());
    glVertex2f(p2.x(), p2.y());
    glEnd();

    if (debug)
    {
        glColor3f(normal.x(), normal.y(), 125);
        glBegin(GL_LINES);
        glVertex2f((p1.x()+p2.x())*0.5f, (p1.y()+p2.y())*0.5f);
        glVertex2f((p1.x()+p2.x())*0.5f + normal.x() * 20,
                   (p1.y()+p2.y())*0.5f + normal.y() * 20);
        glEnd();
        glColor3f(250, 125, 125);
        glBegin(GL_LINES);
        glVertex2f((p1.x()+p2.x())*0.5f, (p1.y()+p2.y())*0.5f);
        glVertex2f((p1.x()+p2.x())*0.5f + tangent.x() * 20,
                   (p1.y()+p2.y())*0.5f + tangent.y() * 20);
        glEnd();
    }
}

void Edge::collision(Actor& other, const QVector2D&)
{
    MovableActor* mActor = dynamic_cast<MovableActor*>(&other);
    if (mActor)
    {
        other.addForce(this->normal * abs(QVector2D::dotProduct(mActor->getNextSpeed(), this->normal)) * elasticity);
        other.addForce(this->tangent * QVector2D::dotProduct(-mActor->getNextSpeed(), this->tangent) * viscosity);
    }
}

const QPointF Edge::getAnchorPoint(QPointF& point) const
{
    QVector2D segToPoint(point-p1);
    float distToPoint = QVector2D::dotProduct(segToPoint, normal);
    return point - normal.toPointF() * (1000.f+distToPoint);
}

QLineF Edge::getLine() const
{
    return QLineF(p1, p2);
}

const QPointF &Edge::point1() const
{
    return p1;
}

const QPointF &Edge::point2() const
{
    return p2;
}
