#ifndef EDGE_H
#define EDGE_H

#include "fixedactor.h"

class Edge : public FixedActor
{
public:
    Edge(const QPointF& p1, const QPointF& p2, const QVector2D& relativeForce = QVector2D(1,1));

private:
    QLineF line;

    QVector2D relativeForce;

    // Actor interface
public:
    void draw(bool debug=false) const;
    void collision(Actor &other) const;
};

#endif // EDGE_H
