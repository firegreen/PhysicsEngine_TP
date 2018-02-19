#include "actor.h"

int Actor::frame = 0;

void Actor::update(float elapsedTime)
{
    UpdateType updateType = this->updateType;
    if (updateType == LeapFrog)
    {
        updateType = (frame % 2) ? Explicit : Implicit;
    }
    if (updateType == Explicit)
    {
        explicitUpdate(elapsedTime);
    }
    else
    {
        implicitUpdate(elapsedTime);
    }
}
