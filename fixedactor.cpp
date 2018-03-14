#include "fixedactor.h"

FixedActor::FixedActor()
{}

bool FixedActor::explicitUpdate(float)
    {return true;}

bool FixedActor::implicitUpdate(float)
    {return true;}

void FixedActor::addForce(const QVector2D&)
{}

void FixedActor::addConstantAcceleration(const QVector2D &)
{}
