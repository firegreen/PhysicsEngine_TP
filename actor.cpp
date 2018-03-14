#include "actor.h"

int Actor::frame = 0;

bool Actor::update(float elapsedTime)
{
    UpdateType updateType = this->updateType;
    if (updateType == LeapFrog)
    {
        updateType = (frame % 2) ? Explicit : Implicit;
    }
    if (updateType == Explicit)
    {
        return explicitUpdate(elapsedTime);
    }
    else
    {
        return implicitUpdate(elapsedTime);
    }
}

void Actor::addForce(float dx, float dy)
{
    addForce(QVector2D(dx,dy));
}

bool Actor::checkCollision(const Actor &other, Intersection &inter) const
{
    if (collider.data() && other.collider.data())
        return collider->checkCollision(*other.collider, inter);
    return false;
}
