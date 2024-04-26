#include "plane.h"


Plane::Plane(){}

// 12.5.2
void Plane::setupBy3Points(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
    Vector3 e3 = p2 - p1;
    Vector3 e1 = p3 - p2;
    Vector3 n = (e3.crossProduct(e1)) / Mag(e3.crossProduct(e1));

    a = n.x;
    b = n.y;
    c = n.z;
    d = p1 * n;
}

Vector3 Plane::normalVector() {
    Vector3 v(a,b,c);
    v.normalize();
    return v;
}

// 12.5.4
_vectorNumType Plane::pointToThisPlaneDistance(const Vector3& point) {
    return point * normalVector() - d;
}

Vector3 computeBestFitNormal(const std::vector<Vector3>& v) {
    Vector3 result = kZeroVector;
    const Vector3 *p = &v[v.size()-1];
    for (int i = 0; i < v.size(); i++) {
        const Vector3 *c = &v[i];
        result.x += (p->z + c->z) * (p->y - c->y);
        result.y += (p->x + c->x) * (p->z - c->z);
        result.z += (p->y + c->y) * (p->x - c->x);
        p = c;
    }
    result.normalize();
    return result;
}