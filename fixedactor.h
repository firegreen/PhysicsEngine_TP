#ifndef FIXEDACTORS_H
#define FIXEDACTORS_H

#include "actor.h"

class FixedActor : public Actor
{
public:
    FixedActor();

    // Actor interface
public:
    void addForce(const QVector2D &force) override;
    void addConstantAcceleration(const QVector2D &force) override;
    bool implicitUpdate(float elapsedTime) override;
    bool explicitUpdate(float elapsedTime) override;

protected:
    //QPointF pos;
};

#endif // FIXEDACTORS_H
