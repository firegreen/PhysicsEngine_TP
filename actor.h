#ifndef ACTOR_H
#define ACTOR_H

#include <QVector2D>
#include <QPointF>

#include "collider.h"

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

    Actor() {}
    virtual ~Actor() {}

    virtual void draw(bool debug=false) const = 0;
    /**
     * @brief update
     * @param elapsedTime elapsed time in seconds
     */
    void update(float elapsedTime);
    virtual void explicitUpdate(float elapsedTime) = 0;
    virtual void implicitUpdate(float elapsedTime) = 0;
    virtual void addForce(const QVector2D& force) = 0;
    virtual void addConstantAcceleration(const QVector2D &force) = 0;
    void checkCollision(const Actor& other) const;
    virtual void collision(Actor& other) const = 0;

    UpdateType updateType = Explicit;

    static int frame;
    Collider* collider;
};

#endif // ACTOR_H
