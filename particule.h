#ifndef PARTICULE_H
#define PARTICULE_H

#include <QColor>
#include "movableactor.h"

class Particle : public MovableActor
{
public:
    Particle();
    Particle(float x, float y, float size = 4.f,
             const QColor& color = Qt::green, float elasticity = 1.5f);
    Particle(const QPointF& pos, float size = 4.f,
             const QColor& color = Qt::green, float elasticity = 1.5f);

    void draw(bool debug=false) const override;
    void collision(Actor& other, const QVector2D& normal) override;

    static int pathResolution;
private:
    float size;
    float elasticity;

    QColor color;
};

#endif // PARTICULE_H
