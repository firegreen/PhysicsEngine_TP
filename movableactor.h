#ifndef MOVABLEACTOR_H
#define MOVABLEACTOR_H

#include "actor.h"

class MovableActor : public Actor
{
public:
    MovableActor(float viscosity = 0.3f);
    MovableActor(QPointF pos, float viscosity = 0.3f);

protected:
    QVector2D acc;
    QList<QVector2D> instantForces;
    QVector2D speed;
    QPointF pos;

    float viscosity;

    // Actor interface
public:
    bool explicitUpdate(float elapsedTime) override;
    bool implicitUpdate(float elapsedTime) override;
    void addForce(const QVector2D &force) override;
    void addConstantAcceleration(const QVector2D &force) override;
    const QPointF getAnchorPoint(QPointF&) const override;

    float x() const;
    float y() const;
    const QPointF& getPosition() const;
    const QVector2D& getSpeed() const;
    const QVector2D getNextSpeed() const;

private:
    QVector2D cachedAcc;
};

#endif // MOVABLEACTOR_H
