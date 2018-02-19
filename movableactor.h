#ifndef MOVABLEACTOR_H
#define MOVABLEACTOR_H

#include "actor.h"

class MovableActor : public Actor
{
public:
    MovableActor();
    MovableActor(QPointF pos);

protected:
    QVector2D acc;
    QVector2D speed;
    QPointF pos;

    // Actor interface
public:
    void explicitUpdate(float elapsedTime) override;
    void implicitUpdate(float elapsedTime) override;
    void addForce(const QVector2D &force) override;
    void addConstantAcceleration(const QVector2D &force) override;

    const QVector2D& getSpeed() const;
};

#endif // MOVABLEACTOR_H
