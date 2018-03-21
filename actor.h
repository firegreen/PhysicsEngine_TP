#ifndef ACTOR_H
#define ACTOR_H

#include <QVector2D>
#include <QPointF>
#include <QSharedPointer>

#include "collider.h"

class Link;

class Actor
{
public:
    enum UpdateType
    {
        Explicit,
        Implicit,
        LeapFrog,
        UpdateTypeCount
    };

    Actor() : elasticity(.05f), glueCoef(0), hardness(0.5) {}
    virtual ~Actor() {}

    virtual void draw(bool debug=false) const = 0;
    /**
     * @brief update
     * @param elapsedTime elapsed time in seconds
     */
    bool update(float elapsedTime);
    virtual bool explicitUpdate(float elapsedTime) = 0;
    virtual bool implicitUpdate(float elapsedTime) = 0;
    virtual void addForce(const QVector2D& force) = 0;
    void addForce(float dx, float dy);
    virtual void addConstantAcceleration(const QVector2D &force) = 0;
    bool checkCollision(const Actor& other, Intersection& inter) const;
    virtual void collision(Actor& other, const QVector2D& normal) = 0;
    virtual const QPointF getAnchorPoint(QPointF& point) const = 0;
    //virtual Link* springForce(Actor& other, const QVector2D& normal);

    UpdateType updateType = Explicit;

    float elasticity;
    float glueCoef;
    float hardness;

    static int frame;
    QSharedPointer<Collider> collider;
};

#endif // ACTOR_H
