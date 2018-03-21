#include "particule.h"
#include "glwidget.h"

#include <iostream>

int Particle::pathResolution = 50;

Particle::Particle()
    : Particle(QPointF(0,0))
{}

Particle::Particle(float x, float y, float size, const QColor &color,
                   float elasticity, float glueCoef, float hardness)
    : Particle(QPointF(x, y), size, color, elasticity, glueCoef, hardness)
{}

Particle::Particle(const QPointF &pos, float size, const QColor &color, float elasticity, float glueCoef, float hardness)
    :  MovableActor(pos), size(size), color(color)
{
    collider = QSharedPointer<Collider>(new CircleCollider(this->pos, size*1.1f));
    this->elasticity = elasticity;
    this->glueCoef = glueCoef;
    this->hardness = hardness;
}

void Particle::draw(bool debug) const
{
    glColor3ub(color.red(), color.green(), color.blue());
    GLWidget::drawEllipse(pos.x(), pos.y(), size, size*5);

    if (debug)
    {
        float time = 0.f, step = 3.f / (float) pathResolution;
        float x1 = pos.x(), y1 = pos.y(), x2, y2;
        for (int i = 0; i < pathResolution; i++)
        {
            time += step;
            x2 = acc.x() * 0.5f * time * time + speed.x() * time + pos.x();
            y2 = acc.y() * 0.5f * time * time + speed.y() * time + pos.y();
            GLWidget::drawLine(x1,y1,x2,y2);
            x1 = x2;
            y1 = y2;
        }
    }
}

void Particle::collision(Actor &other, const QVector2D &normal)
{
    MovableActor* mActor = dynamic_cast<MovableActor*>(&other);

    if (mActor)
        other.addForce(normal * abs(QVector2D::dotProduct(mActor->getNextSpeed(), normal)) * elasticity);
    addForce(-normal * abs(QVector2D::dotProduct(getSpeed(), normal)) * (elasticity-1.f) * 0.5f);
}
