#include "spring.h"
#include "glwidget.h"
#include "movableactor.h"
#include "GL/glu.h"

Spring::Spring(Actor& a1, Actor& a2, const QPointF &p1, const QPointF &p2,
               float k, float l, float resistance, bool display)
    : a1(a1), a2(a2), p1(p1), p2(p2), k(k), l0(l), resistance(resistance), display(display)
{
    QLineF vector(p1, p2);
    tangent = QVector2D(vector.dx(), vector.dy()).normalized();
    vector = vector.normalVector();
    normal = -QVector2D(vector.dx(), vector.dy()).normalized();
    //collider = QSharedPointer<Collider>(new LineCollider(this->p1, this->p2));
    if (!l0)
        l0 = vector.length();
    MovableActor* actor = dynamic_cast<MovableActor*>(&a1);
    if (actor)
        currentA1Pos = actor->getPosition();
    actor = dynamic_cast<MovableActor*>(&a2);
    if (actor)
        currentA2Pos = actor->getPosition();
}

Spring::Spring(Actor& a1, Actor& a2, float k, float l, float resistance, bool display)
    : a1(a1), a2(a2), p1(p1), p2(p2), k(k), l0(l), resistance(resistance), display(display)
{
    QLineF vector(p1, p2);
    tangent = QVector2D(vector.dx(), vector.dy()).normalized();
    vector = vector.normalVector();
    normal = -QVector2D(vector.dx(), vector.dy()).normalized();
    //collider = QSharedPointer<Collider>(new LineCollider(this->p1, this->p2));
    if (!l0)
        l0 = vector.length();
    MovableActor* actor = dynamic_cast<MovableActor*>(&a1);
    if (actor)
        currentA1Pos = actor->getPosition();
    actor = dynamic_cast<MovableActor*>(&a2);
    if (actor)
        currentA2Pos = actor->getPosition();
}

void Spring::draw(bool) const
{
    glBegin(GL_LINES);
    glVertex2f(p1.x(), p1.y());
    glVertex2f(p2.x(), p2.y());
    glEnd();
}

bool Spring::update(float elapsedTime)
{
    MovableActor* actor = dynamic_cast<MovableActor*>(&a1);
    if (actor)
    {
        p1 += actor->getPosition() - currentA1Pos;
        currentA1Pos= actor->getPosition();
    }
    actor = dynamic_cast<MovableActor*>(&a2);
    if (actor)
    {
        p2 += actor->getPosition() - currentA2Pos;
        currentA2Pos= actor->getPosition();
    }

    //MovableActor::explicitUpdate(elapsedTime);
    QLineF vector(p1, p2);
    float l = vector.length();
    float diff = l0 - l;

    if (-diff / l > resistance)
    {
        return false;
    }

    QVector2D force(vector.dx() / l, vector.dy() / l);
    a1.addForce(force * -diff * elapsedTime * 1.f/k);
    a2.addForce(force * diff * elapsedTime * 1.f/k);
    return true;
}
