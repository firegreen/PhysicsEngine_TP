#include "movableactor.h"

MovableActor::MovableActor()
{}

MovableActor::MovableActor(QPointF pos)
    : pos(pos)
{}

const float _epsilon = 0.001f;

void MovableActor::explicitUpdate(float elapsedTime)
{
    if (acc.lengthSquared() < _epsilon)
        acc = QVector2D(0,0);
    if (speed.lengthSquared() < _epsilon)
        speed = QVector2D(0,0);
    pos = pos + speed.toPointF() * elapsedTime;
    speed += acc * elapsedTime;
}

void MovableActor::implicitUpdate(float elapsedTime)
{
    if (acc.lengthSquared() < _epsilon)
        acc = QVector2D(0,0);
    if (speed.lengthSquared() < _epsilon)
        speed = QVector2D(0,0);
    speed += acc * elapsedTime;
    pos = pos + speed.toPointF() * elapsedTime;
}

void MovableActor::addForce(const QVector2D &force)
{
    speed += force;
}

void MovableActor::addConstantAcceleration(const QVector2D &force)
{
    this->acc += force;
}

const QVector2D& MovableActor::getSpeed() const
{
    return speed;
}
