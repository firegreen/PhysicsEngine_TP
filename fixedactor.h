#ifndef FIXEDACTORS_H
#define FIXEDACTORS_H

#include "actor.h"

class FixedActor : public Actor
{
public:
    FixedActor();

    // Actor interface
public:
    void explicitUpdate(float elapsedTime) override;
    void addForce(const QVector2D &force) override;
    void addConstantAcceleration(const QVector2D &force) override;
    void implicitUpdate(float elapsedTime) override;
};

#endif // FIXEDACTORS_H
