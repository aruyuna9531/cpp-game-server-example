#ifndef VECTOR3_H_
#define VECTOR3_H_

#include<assert.h>
#include<string>
#include "vector_num_type.h"

class Vector3 {
public:
    _vectorNumType x,y,z;
    Vector3();
    Vector3(const Vector3& a);
    Vector3(const _vectorNumType& _x, const _vectorNumType& _y, const _vectorNumType& _z);
    Vector3& operator=(const Vector3& a);
    bool operator==(const Vector3& a) const;
    bool operator!=(const Vector3& a) const;
    void zero();
    // 取反
    Vector3 operator-() const;
    Vector3 operator+(const Vector3& a) const;
    Vector3 operator-(const Vector3& a) const;

    // 标量乘除
    Vector3 operator*(const _vectorNumType& a) const;
    Vector3 operator/(const _vectorNumType& a) const;
    // 向量点乘
    _vectorNumType operator*(const Vector3& a) const;

    // 模
    _vectorNumType Mag() const;

    // 叉乘
    Vector3 crossProduct(const Vector3& b) const;

    std::string Print() const;

    void normalize();
};

// 求模 非成员版
extern _vectorNumType Mag(const Vector3 &a);
// 标量×向量
extern Vector3 operator*(const _vectorNumType& k, const Vector3& v);

// 两点距离
extern _vectorNumType distance(const Vector3& a, const Vector3& b);

// 两点距离的平方
extern _vectorNumType distanceSquared(const Vector3& a, const Vector3& b);

extern const Vector3 kZeroVector;
#endif