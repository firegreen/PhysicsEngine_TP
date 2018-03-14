#include "movableactor.h"

MovableActor::MovableActor(float viscosity)
    : MovableActor(QPointF(), viscosity)
{}

MovableActor::MovableActor(QPointF pos, float viscosity)
    :  pos(pos), viscosity(viscosity)
{}

const float _epsilon = 0.001f;

bool MovableActor::explicitUpdate(float elapsedTime)
{
    if (acc.lengthSquared() < _epsilon)
        acc = QVector2D(0,0);
    if (cachedAcc.lengthSquared() < _epsilon)
        cachedAcc = QVector2D(0,0);

    for (QVector2D force : instantForces)
       speed += force;
    pos = pos + speed.toPointF() * elapsedTime;
    speed += cachedAcc;
    cachedAcc = acc * elapsedTime;
    cachedAcc -= viscosity * speed * elapsedTime;

    instantForces.clear();
    return true;
}

bool MovableActor::implicitUpdate(float elapsedTime)
{
    if (acc.lengthSquared() < _epsilon)
        acc = QVector2D(0,0);
    if (speed.lengthSquared() < _epsilon)
        speed = QVector2D(0,0);

    for (QVector2D force : instantForces)
       speed += force;
    speed += cachedAcc;
    cachedAcc = acc * elapsedTime;
    cachedAcc -= viscosity * speed * elapsedTime;
    pos = pos + speed.toPointF() * elapsedTime;

    instantForces.clear();
    return true;
}

void MovableActor::addForce(const QVector2D &force)
{
    instantForces.append(force);
}

void MovableActor::addConstantAcceleration(const QVector2D &force)
{
    this->acc += force;
}

float MovableActor::x() const
{
    return pos.x();
}

float MovableActor::y() const
{
    return pos.y();
}

const QPointF &MovableActor::getPosition() const
{
    return pos;
}

const QVector2D& MovableActor::getSpeed() const
{
    return speed;
}

const QVector2D MovableActor::getNextSpeed() const
{
    return speed+cachedAcc;
}
