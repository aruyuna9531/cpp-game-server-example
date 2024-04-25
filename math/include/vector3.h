#ifndef VECTOR3_H_
#define VECTOR3_H_

#include<assert.h>
#include "vector_num_type.h"

class Vector3 {
public:
    _vectorNumType x,y,z;
    Vector3(){}
    Vector3(const Vector3& a): x(a.x), y(a.y), z(a.z){}
    Vector3(const _vectorNumType& _x, const _vectorNumType& _y, const _vectorNumType& _z): x(_x), y(_y), z(_z){}
    Vector3& operator=(const Vector3& a) {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }
    bool operator==(const Vector3& a) const {
        return x == a.x && y == a.y && z == a.z;
    }
    bool operator!=(const Vector3& a) const {
        return x != a.x && y != a.y && z != a.z;
    }
    void zero() {
        x = y = z = 0;
    }
    // 取反
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }
    Vector3 operator+(const Vector3& a) const {
        return Vector3(x+a.x, y+a.y, z+a.z);
    }
    Vector3 operator-(const Vector3& a) const {
        return Vector3(x-a.x, y-a.y, z-a.z);
    }

    // 标量乘除
    Vector3 operator*(const _vectorNumType& a) const {
        return Vector3(x*a, y*a, z*a);
    }
    Vector3 operator/(const _vectorNumType& a) const {
        assert(a != 0);
        return Vector3(x/a, y/a, z/a);
    }
    // 向量乘除
    _vectorNumType operator*(const Vector3& a) const {
        return x * a.x + y * a.y + z * a.z;
    }

    // 模
    _vectorNumType Mag() const {
        return sqrt(x*x+y*y+z*z);
    }

    // 叉乘
    Vector3 crossProduct(const Vector3& b) const {
        return Vector3(
            y * b.z - z * b.y,
            z * b.x - x * b.z,
            x * b.y - y * b.x
        );
    }
};

// 求模 非成员版
inline _vectorNumType Mag(const Vector3 &a) {
        return a.Mag();
    }

// 标量×向量
inline Vector3 operator*(const _vectorNumType& k, const Vector3& v) {
    return Vector3(k*v.x, k*v.y, k*v.z);
}

// 两点距离
inline _vectorNumType distance(const Vector3& a, const Vector3& b) {
    _vectorNumType dx = a.x - b.x;
    _vectorNumType dy = a.y - b.y;
    _vectorNumType dz = a.z - b.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

extern const Vector3 kZeroVector;
#endif