#ifndef EDGE_H
#define EDGE_H

#include "fixedactor.h"
#include <QColor>

class Edge : public FixedActor
{
public:
    Edge(const QPointF& p1, const QPointF& p2 , const QColor &color = Qt::black,
         const float elasticity = 1.001f, const float viscosity = .03f);

    void draw(bool debug=false) const;
    void collision(Actor &other, const QVector2D& normal) override;

    QLineF getLine() const;

private:
    QPointF p1, p2;
    QVector2D normal;
    QVector2D tangent;

    QColor color;

    float elasticity;
    float viscosity;
};

#endif // EDGE_H
