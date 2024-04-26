#ifndef AABB3_H_
#define AABB3_H_
#include "vector3.h"
class Matrix4x3;

// AABB模型：物体的边界立方体，立方体边界和坐标轴平行。（不平行的叫OBB）
class AABB3 {
public:
    Vector3 min, max;

    Vector3 size() const;
    _vectorNumType xSize() const;
    _vectorNumType ySize() const;
    _vectorNumType zSize() const;
    Vector3 center() const;

    // 获得顶点
    Vector3 corner(int i) const;

    void empty();   // 清空

    void add(const Vector3& p); // 向矩形边界框添加点
    void add(const AABB3& p); // 向矩形边界框添加AABB

    void setToTransformedBox(const AABB3& box, const Matrix4x3& m);

    bool isEmpty() const;   // 矩形边界框为空返回true
    bool contains(const Vector3& p) const;  // 矩形边界框包含p返回true;
    Vector3 closestPointTo(const Vector3& p) const; // 矩形边界框上的最近点
    bool intersectsSphere(const Vector3& center,const  _vectorNumType& radius) const;  // 判断本AABB与输入球体相交
    _vectorNumType rayIntersect(const Vector3& rayOrg, const Vector3& rayDelta, Vector3* returnNormal = nullptr) const; //与射线相交
    int classifyPlane(const Vector3& n, const _vectorNumType& d) const;    // 判断矩形边界框在平面的那一面
    _vectorNumType intersectPlane(const Vector3& n, const _vectorNumType& planeD, const Vector3& dir) const;
};

// 两个AABB相交返回true，boxIntersect返回相交部分
extern bool intersectAABBs(const AABB3& box1, const AABB3& box2, AABB3* boxIntersect = nullptr);

// 运动AABB和静止AABB相交时的参数t，不想交时返回值>1
extern _vectorNumType intersectMovingAABB(const AABB3& stationaryBox, const AABB3& movingBox,const Vector3& d);

extern const _vectorNumType kNoIntersection;
#endif