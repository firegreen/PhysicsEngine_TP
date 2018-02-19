#include "collider.h"

bool CircleCollider::checkCollision(const Collider &other) const
{
    if(other.type == Circle)
    {
        const CircleCollider& circle = reinterpret_cast<const CircleCollider&>(other);
        return false;
    }
    if(other.type == Line)
    {
        const LineCollider& line = reinterpret_cast<const LineCollider&>(other);
        return false;
    }
    return false;
}


bool LineCollider::checkCollision(const Collider &other) const
{
    if(other.type == Circle)
    {
        const CircleCollider& circle = reinterpret_cast<const CircleCollider&>(other);
        return false;
    }
    if(other.type == Line)
    {
        const LineCollider& line = reinterpret_cast<const LineCollider&>(other);
        return false;
    }
    return false;
}
