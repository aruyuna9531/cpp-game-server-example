#include "triangle.h"
#include "vector3.h"

Triangle::Triangle(){}
Triangle::Triangle(const Vector3& _p1,const Vector3& _p2,const Vector3& _p3): p1(_p1), p2(_p2), p3(_p3) {}

_vectorNumType Triangle::Length() {
    return Mag(p3-p2) + Mag(p2-p1) + Mag(p1-p3);
}

_vectorNumType Triangle::Area() {
    return (p3-p2).crossProduct(p2 - p1).Mag() / 2;
}

Vector3 Triangle::BarycentricCoords3D(const Vector3& p) {
    Vector3 d1 = p2 - p1;
    Vector3 d2 = p3 - p2;
    Vector3 n = d1.crossProduct(d2);    // 这里n不需要正则化

    _vectorNumType u1,u2,u3,u4;
    _vectorNumType v1,v2,v3,v4;
    if ((fabs(n.x) >= fabs(n.y)) && (fabs(n.x) >= fabs(n.z))) {
        // 抛弃x，向yz平面投影
        u1 = p1.y - p3.y;
        u2 = p2.y - p3.y;
        u3 = p.y - p1.y;
        u4 = p.y - p3.y;
        v1 = p1.z - p3.z;
        v2 = p2.z - p3.z;
        v3 = p.z - p1.z;
        v4 = p.z - p3.z;
    } else if (fabs(n.y) >= fabs(n.z)) {
        // 抛弃y，向xz平面投影
        u1 = p1.z - p3.z;
        u2 = p2.z - p3.z;
        u3 = p.z - p1.z;
        u4 = p.z - p3.z;
        v1 = p1.x - p3.x;
        v2 = p2.x - p3.x;
        v3 = p.x - p1.x;
        v4 = p.x - p3.x;
    } else {
        u1 = p1.x - p3.x;
        u2 = p2.x - p3.x;
        u3 = p.x - p1.x;
        u4 = p.x - p3.x;
        v1 = p1.y - p3.y;
        v2 = p2.y - p3.y;
        v3 = p.y - p1.y;
        v4 = p.y - p3.y;
    }

    _vectorNumType denom = v1 * u2 - v2 * u1;
    if (denom == 0) {
        return kZeroVector;  // 面积为0
    }

    _vectorNumType oneOverDenum = 1 / denom;
    Vector3 result;
    result.x = (v4 * u2 - v2 * u4) * oneOverDenum;
    result.y = (v1 * u3 - v3 * u1) * oneOverDenum;
    result.z = 1 - result.x - result.y;
    return result;
}

Vector3 Triangle::BaryCenter() {
    return (p1 + p2 + p3) / 3;
}
Vector3 Triangle::InnerHeart() {
    _vectorNumType l1 = (p3-p2).Mag();
    _vectorNumType l2 = (p3-p1).Mag();
    _vectorNumType l3 = (p2-p1).Mag();
    return (l1 * p1 + l2 * p2 + l3 * p3) / (l1 + l2 + l3);
}
Vector3 Triangle::OuterHeart() {
    Vector3 e1 = p3 - p2;
    Vector3 e2 = p1 - p3;
    Vector3 e3 = p2 - p1;
    _vectorNumType d1 = -(e2 * e3);
    _vectorNumType d2 = -(e3 * e1);
    _vectorNumType d3 = -(e1 * e2);
    _vectorNumType c1 = d2 * d3;
    _vectorNumType c2 = d3 * d1;
    _vectorNumType c3 = d1 * d2;
    _vectorNumType c = c1 + c2 + c3;
    return ((c2 + c3) * p1 + (c3 + c1) * p2 + (c1 + c2) * p3) / (2 * c);
}