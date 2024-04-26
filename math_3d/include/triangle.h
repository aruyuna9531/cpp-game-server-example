#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include "vector_num_type.h"
#include "vector3.h"
class Triangle{
public:
    Vector3 p1, p2, p3; // 3个顶点
    Triangle();
    Triangle(const Vector3& _p1,const Vector3& _p2,const Vector3& _p3);

    _vectorNumType Length();    // 周长
    _vectorNumType Area();      // 面积

    // 三角形基于p的重心坐标
    Vector3 BarycentricCoords3D(const Vector3& p);

    Vector3 BaryCenter();   // 三角形自己的重心（中线交点）
    Vector3 InnerHeart();   // 内心
    Vector3 OuterHeart();   // 外心
};
#endif