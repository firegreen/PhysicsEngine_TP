#ifndef SPRING_H
#define SPRING_H

#include "movableactor.h"

class Spring
{
public:
    Spring(Actor &a1, Actor &a2, const QPointF& p1, const QPointF& p2,
           float k=1.5, float l0=0, float resistance=0.7);
    virtual ~Spring() {}

    virtual void draw(bool debug=false) const;

    virtual bool update(float elapsedTime);

    Actor& a1;
    Actor& a2;

    QPointF p1, p2;
    QPointF currentA1Pos, currentA2Pos;
    QVector2D normal;
    QVector2D tangent;

    float k, l0;
    float resistance;

    bool display;
};
#endif // SPRING_H
