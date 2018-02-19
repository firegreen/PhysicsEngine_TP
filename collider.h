#ifndef COLLIDER_H
#define COLLIDER_H

#include <QPointF>

class Collider
{
public:
    enum Type
    {
        Line,
        Circle,
        TypeCount
    };

    Collider(Type type) : type(type) {}
    virtual ~Collider() {}

    virtual bool checkCollision(const Collider& other) const = 0;

    const Type type;
};

class CircleCollider : public Collider
{
public:
    CircleCollider(QPointF& center, float radius) : Collider(Circle), center(center), radius(radius)
        {}
    QPointF& center;
    float radius;

    // Collider interface
public:
    bool checkCollision(const Collider &other) const;
};

class LineCollider : public Collider
{
public:
    LineCollider(QPointF& p1, QPointF& p2) : Collider(Line), p1(p1), p2(p2)
        {}
    QPointF& p1;
    QPointF& p2;

// Collider interface
public:
    bool checkCollision(const Collider &other) const;
};



#endif // COLLIDER_H
