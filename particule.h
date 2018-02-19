#ifndef PARTICULE_H
#define PARTICULE_H

#include <QColor>
#include "movableactor.h"

class Particle : public MovableActor
{
public:
    Particle();
    Particle(float x, float y, float size = 4.f, const QColor& color = Qt::green);
    Particle(const QPointF& pos, float size = 4.f, const QColor& color = Qt::green);

    void draw(bool debug=false) const override;
    void collision(Actor& other) const override;

    static int pathResolution;
private:
    float size;

    QColor color;
};

#endif // PARTICULE_H
