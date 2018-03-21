#ifndef SPRING_H
#define SPRING_H

#include "movableactor.h"
#include <QWeakPointer>

class Link
{
public:
    Link(QSharedPointer<Actor> a1, QSharedPointer<Actor> a2, const QPointF& p1, const QPointF& p2,
           float k=1.5, float l0=0, float resistance=0.7, bool active=true, bool display=true);
    Link(QSharedPointer<Actor> a1, QSharedPointer<Actor> a2, float k=1.5, float l0=0,
         float resistance=0.7, bool active=true, bool display=true);
    virtual ~Link() {}

    virtual void draw(bool debug=false) const;

    virtual bool update(float elapsedTime);
    void updatePositions();

    QWeakPointer<Actor> a1;
    QWeakPointer<Actor> a2;

    QPointF p1, p2;
    QPointF currentA1Pos, currentA2Pos;

    float k, l0;
    float resistance;

    bool active;
    bool display;

};

class HardLink : public Link
{
public:
    HardLink(QSharedPointer<Actor> a1, QSharedPointer<Actor> a2, const QPointF& p1, const QPointF& p2,
           float k=1.5, float l0=0, float resistance=0.7, bool display=true);

    virtual void draw(bool debug=false) const;
    virtual bool update(float elapsedTime);

};
#endif // SPRING_H
