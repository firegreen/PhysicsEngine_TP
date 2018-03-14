#include <math.h>

#include "collider.h"
#define EPSILON 0.0005f
#define PI 3.141592654f
#define isZero(number) (number > -EPSILON && number < EPSILON)

int collision_D_S(const QPointF& A, const QPointF& B, const QPointF& P1, const QPointF& P2)
{
    QPointF vAP1(P1 - A),vAP2(P2 - A),vAB(B - A);
    float d1 = vAB.x()*vAP1.y() - vAB.y()*vAP1.x();
    float d2 = vAB.x()*vAP2.y() - vAB.y()*vAP2.x();
    return d1*d2<0;
}

bool CircleCollider::checkCollision(const Collider &other, Intersection &i) const
{
    if(other.type == Circle)
    {
        const CircleCollider& circle = reinterpret_cast<const CircleCollider&>(other);
        const QPointF& C1 = center;
        const QPointF& C2 = circle.center;

        float distanceX = C1.x() - C2.x();
        float distanceY = C1.y() - C2.y();

        float radiusSum = radius + circle.radius;
        if(distanceX*distanceX+distanceY*distanceY<radiusSum*radiusSum){
            i.normal1 = QVector2D(distanceX,distanceY);
            i.normal1.normalize();
            i.normal2 = i.normal1 * -1.f;
            i.point = C2 + i.normal1.toPointF() * circle.radius;
            return true;
        }
    }
    if(other.type == Line)
    {
        const LineCollider& line = reinterpret_cast<const LineCollider&>(other);
        QPointF AB(line.p2 - line.p1);
        QPointF CA(line.p1 - center);
        float scalAB = AB.x()*AB.x() + AB.y() * AB.y();
        float scalABCA = AB.x()*CA.x() + AB.y() * CA.y();
        float scalOA = line.p1.x()*line.p1.x()+ line.p1.y()*line.p1.y();
        float scalOC = center.x()*center.x()+center.y()*center.y();
        float scalOCOA = line.p1.x()*center.x()+center.y()*line.p1.y();

        float a = scalAB;
        float b = scalABCA * 2.f;
        float c = scalOA + scalOC - 2.f*(scalOCOA) - radius*radius;
        float delta = b*b - 4.f*a*c;
        if(delta>0.f){
            float t1 = (-b + sqrtf(delta))/(2.f*a);
            float t2 = (-b - sqrtf(delta))/(2.f*a);
            if(t1>t2)
            {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }
            if((t1 <= 0.f && t2 >=1.f) || (t2>=0.f && t2 <=1.f) || (t1 >=0.f && t1<=1.f))
            {
                i.point = line.p1 + AB * (t1+t2)*0.5f;
                i.normal1 = QVector2D(AB.y(),-AB.x());
                i.normal1.normalize();
                i.normal2 = i.normal1 * -1.f;
                return true;
            }
        }
        else if(isZero(delta)){
            float t = (-b)/(2.*a);
            if(t>=0. && t<=1.){
                i.point = line.p1 + AB * t;
                i.normal1 = QVector2D(AB.y(),-AB.x());
                i.normal1.normalize();
                i.normal2 = i.normal1 * -1.f;
                return true;
            }
        }
    }
    return false;
}


bool LineCollider::checkCollision(const Collider &other, Intersection &i) const
{
    if(other.type == Circle)
    {
        const CircleCollider& circle = reinterpret_cast<const CircleCollider&>(other);
        QPointF AB(p2 - p1);
        QPointF CA(p1 - circle.center);
        float scalAB = AB.x()*AB.x() + AB.y() * AB.y();
        float scalABCA = AB.x()*CA.x() + AB.y() * CA.y();
        float scalOA = p1.x()*p1.x()+ p1.y()*p1.y();
        float scalOC = circle.center.x()*circle.center.x()+circle.center.y()*circle.center.y();
        float scalOCOA = p1.x()*circle.center.x()+circle.center.y()*p1.y();

        float a = scalAB;
        float b = scalABCA * 2.f;
        float c = scalOA + scalOC - 2.f*(scalOCOA) - circle.radius*circle.radius;
        float delta = b*b - 4.f*a*c;
        if(delta>0.f){
            float t1 = (-b + sqrtf(delta))/(2.f*a);
            float t2 = (-b - sqrtf(delta))/(2.f*a);
            if(t1>t2)
            {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }
            float t;
            if((t1 <= 0.f && t2 >=1.f) || (t2 <= 0.f && t1 >=1.f))
                t = 0.5f;
            else if (t2>=0.f && t2 <=1.f)
                t = t2;
            else if(t1 >=0.f && t1<=1.f)
                t = t1;
            else
                return false;

            i.point = QPointF(p1.x() + AB.x()*t, p1.y() + AB.y()*t);
            i.normal1 = QVector2D(i.point - circle.center);
            i.normal1.normalize();
            i.normal2 = i.normal1 * -1.f;
            return true;
        }
        else if(isZero(delta)){
            float t = (-b)/(2.*a);
            if(t>=0. && t<=1.){
                i.point = QPointF(p1.x() + AB.x()*t, p1.y() + AB.y()*t);
                i.normal1 = QVector2D(i.point - circle.center);
                i.normal1.normalize();
                i.normal2 = i.normal1 * -1.f;
                return true;
            }
        }
    }
    if(other.type == Line)
    {
        const LineCollider& line = reinterpret_cast<const LineCollider&>(other);
        if(!collision_D_S(p1, p2, line.p1,line.p2))
            return false;
        QPointF v0P(line.p2-line.p1),vAB(p2-p1);
        float k = -(p1.x()*v0P.y()-line.p1.x()*v0P.y()-v0P.x()*p1.y()+v0P.x()*p1.y())
                /(vAB.x()*v0P.y()-vAB.y()*v0P.x());
        if (k>0. && k<1.){
            i.point = p1 + k * vAB;
            i.normal1 = QVector2D(v0P.y(),-v0P.x());
            i.normal1.normalize();
            i.normal2 = i.normal1 * -1;
            return true;
        }
    }
    return false;
}
