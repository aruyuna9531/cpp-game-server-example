#ifndef PLANE_H_
#define PLANE_H_
#include "vector_num_type.h"
#include "vector3.h"
#include <vector>

// 平面
class Plane {
    _vectorNumType a,b,c,d;     // 平面方程ax+by+cz=d决定的平面
    Plane();
    void setupBy3Points(const Vector3& a, const Vector3& b, const Vector3& c);

    // 法向量
    Vector3 normalVector();

    // 空间一点到本平面距离
    _vectorNumType pointToThisPlaneDistance(const Vector3& point);
};

// 12.5.3
extern Vector3 computeBestFitNormal(const std::vector<Vector3>& v);
#endif