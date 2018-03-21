#include "link.h"
#include "glwidget.h"
#include "movableactor.h"
#include "GL/glu.h"

#include <iostream>

Link::Link(QSharedPointer<Actor> a1, QSharedPointer<Actor> a2, const QPointF &p1, const QPointF &p2,
               float k, float l, float resistance, bool active, bool display)
    : a1(a1), a2(a2), p1(p1), p2(p2), k(k), l0(l), resistance(resistance), active(active), display(display)
{
    updatePositions();
}

Link::Link(QSharedPointer<Actor> a1, QSharedPointer<Actor> a2, float k, float l, float resistance, bool active, bool display)
    : a1(a1), a2(a2), k(k), l0(l), resistance(resistance), active(active), display(display)
{}

void Link::updatePositions()
{
    QLineF vector(p1, p2);
    if (!l0)
        l0 = vector.length();
    MovableActor* actor = dynamic_cast<MovableActor*>(a1.data());
    if (actor)
        currentA1Pos = actor->getPosition();
    actor = dynamic_cast<MovableActor*>(a2.data());
    if (actor)
        currentA2Pos = actor->getPosition();
}

void Link::draw(bool) const
{

    if (display && !p1.isNull() && !p2.isNull())
    {
        glBegin(GL_LINES);
        glVertex2f(p1.x(), p1.y());
        glVertex2f(p2.x(), p2.y());
        glEnd();
    }
}

bool Link::update(float elapsedTime)
{
    if (a1.isNull() || a2.isNull())
        return false;
    if (!active)
    {
        Intersection intersection;
        if (a1.data()->checkCollision(*a2.data(), intersection))
        {
            p1 = a1.data()->getAnchorPoint(intersection.point);
            p2 = a2.data()->getAnchorPoint(intersection.point);

            if (MovableActor* actor = dynamic_cast<MovableActor*>(a1.data()))
            {
                float elasticiy = std::max(0.f,a1.data()->elasticity - a1.data()->glueCoef);
                p1 += intersection.normal2.toPointF() * actor->getSpeed().length() * elapsedTime * elasticiy;
            }
            if (MovableActor* actor = dynamic_cast<MovableActor*>(a2.data()))
            {
                float elasticiy = std::max(0.f,a2.data()->elasticity - a2.data()->glueCoef);
                p2 += intersection.normal1.toPointF() * actor->getSpeed().length() * elapsedTime * elasticiy;
            }
            updatePositions();

            resistance = a1.data()->glueCoef * a2.data()->glueCoef;
            k = elapsedTime*(1/(a1.data()->hardness*a2.data()->hardness))*(1+a1.data()->glueCoef*a2.data()->glueCoef*0.5);

            active = true;
            display = false;
        }
    }
    if (std::abs(l0) < 0.001f)
    {
        active = false;
    }
    if (active && !p1.isNull() && !p2.isNull())
    {
        MovableActor* actor = dynamic_cast<MovableActor*>(a1.data());
        if (actor)
        {
            p1 += actor->getPosition() - currentA1Pos;
            currentA1Pos= actor->getPosition();
        }
        actor = dynamic_cast<MovableActor*>(a2.data());
        if (actor)
        {
            p2 += actor->getPosition() - currentA2Pos;
            currentA2Pos= actor->getPosition();
        }
        p1 = a1.data()->getAnchorPoint(p2);
        p2 = a2.data()->getAnchorPoint(p1);

        //MovableActor::explicitUpdate(elapsedTime);
        QLineF vector(p1, p2);
        float l = vector.length();
        float diff = l0 - l;

        QVector2D force(vector.dx() / l, vector.dy() / l);
        a1.data()->addForce(0.8 * force * -diff * elapsedTime/k);
        a2.data()->addForce(0.8 * force * diff * elapsedTime/k);

        if (-diff / l0 > resistance)
        {
            active = false;
            p2 = p1 = QPointF();
            l0 = 0;
        }
        resistance = a1.data()->glueCoef * a2.data()->glueCoef;
        k = elapsedTime*(1/(a1.data()->hardness*a2.data()->hardness))*(1+a1.data()->glueCoef*a2.data()->glueCoef*0.5);
    }
    return true;
}

HardLink::HardLink(QSharedPointer<Actor> a1, QSharedPointer<Actor> a2, const QPointF &p1, const QPointF &p2,
               float k, float l, float resistance, bool display)
    : Link(a1, a2, p1, p2, k, l, resistance, true, display)
{}

void HardLink::draw(bool) const
{
    if (display)
    {
        glBegin(GL_LINES);
        glVertex2f(p1.x(), p1.y());
        glVertex2f(p2.x(), p2.y());
        glEnd();
    }
}

bool HardLink::update(float elapsedTime)
{
    if (a1.isNull() || a2.isNull())
        return false;
    if (active)
    {
        MovableActor* actor = dynamic_cast<MovableActor*>(a1.data());
        if (actor)
        {
            p1 += actor->getPosition() - currentA1Pos;
            currentA1Pos= actor->getPosition();
        }
        actor = dynamic_cast<MovableActor*>(a2.data());
        if (actor)
        {
            p2 += actor->getPosition() - currentA2Pos;
            currentA2Pos= actor->getPosition();
        }

        //MovableActor::explicitUpdate(elapsedTime);
        QLineF vector(p1, p2);
        float l = vector.length();
        float diff = l0 - l;

        QVector2D force(vector.dx() / l, vector.dy() / l);
        a1.data()->addForce(force * -diff * 0.9 * elapsedTime * 1.f/k);
        a2.data()->addForce(force * diff * 0.9 * elapsedTime * 1.f/k);

        if (-diff / l0 > resistance)
        {
            active = false;
            p2 = p1 = QPointF();
            return false;
        }
    }
    return true;
}

