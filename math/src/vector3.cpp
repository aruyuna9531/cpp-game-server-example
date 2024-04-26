#include "vector3.h"
#include <assert.h>
#include <string>
Vector3::Vector3(){}
Vector3::Vector3(const Vector3& a): x(a.x), y(a.y), z(a.z){}
Vector3::Vector3(const _vectorNumType& _x, const _vectorNumType& _y, const _vectorNumType& _z): x(_x), y(_y), z(_z){}
Vector3& Vector3::operator=(const Vector3& a) {
    x = a.x;
    y = a.y;
    z = a.z;
    return *this;
}
bool Vector3::operator==(const Vector3& a) const {
    return x == a.x && y == a.y && z == a.z;
}
bool Vector3::operator!=(const Vector3& a) const {
    return x != a.x && y != a.y && z != a.z;
}
void Vector3::zero() {
    x = y = z = 0;
}
// 取反
Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator+(const Vector3& a) const {
    return Vector3(x+a.x, y+a.y, z+a.z);
}
Vector3 Vector3::operator-(const Vector3& a) const {
    return Vector3(x-a.x, y-a.y, z-a.z);
}

// 标量乘除
Vector3 Vector3::operator*(const _vectorNumType& a) const {
    return Vector3(x*a, y*a, z*a);
}
Vector3 Vector3::operator/(const _vectorNumType& a) const {
    assert(a != 0);
    return Vector3(x/a, y/a, z/a);
}
// 向量乘除
_vectorNumType Vector3::operator*(const Vector3& a) const {
    return x * a.x + y * a.y + z * a.z;
}

// 模
_vectorNumType Vector3::Mag() const {
    return sqrt(x*x+y*y+z*z);
}

// 叉乘
Vector3 Vector3::crossProduct(const Vector3& b) const {
    return Vector3(
        y * b.z - z * b.y,
        z * b.x - x * b.z,
        x * b.y - y * b.x
    );
}

std::string Vector3::Print() const {
    std::string s;
    s += "(" + std::to_string(x.value) + "," + std::to_string(y.value) + "," + std::to_string(z.value) + ")";
    return s;
}

void Vector3::normalize() {
    _vectorNumType m = Mag();
    assert(m != 0);
    x = x / m;
    y = y / m;
    z = z / m;
}

// 求模 非成员版
_vectorNumType Mag(const Vector3 &a) {
        return a.Mag();
    }

// 标量×向量
Vector3 operator*(const _vectorNumType& k, const Vector3& v) {
    return Vector3(k*v.x, k*v.y, k*v.z);
}

// 两点距离
_vectorNumType distance(const Vector3& a, const Vector3& b) {
    return sqrt(distanceSquared(a,b));
}

_vectorNumType distanceSquared(const Vector3& a, const Vector3& b) {
    _vectorNumType dx = a.x - b.x;
    _vectorNumType dy = a.y - b.y;
    _vectorNumType dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}
