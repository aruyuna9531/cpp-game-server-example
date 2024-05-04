
// 方便浮点数==用的 注意会带来的精度丢失问题
#ifndef VECTOR_NUM_TYPE_H_
#define VECTOR_NUM_TYPE_H_

#include <iostream>
#include <cmath>
#include <assert.h>

struct _vectorNumType {
    double value;
    _vectorNumType();
    _vectorNumType(const double& _v);
    _vectorNumType& operator=(const double& _v);
    // 差值误差在这个范围内就认为相等
    bool operator==(const _vectorNumType& other) const;
    bool operator>(const _vectorNumType& other) const;
    bool operator<(const _vectorNumType& other) const;
    bool operator!=(const _vectorNumType& other) const;
    bool operator>=(const _vectorNumType& other) const;
    bool operator<=(const _vectorNumType& other) const;
    _vectorNumType operator-() const;
    _vectorNumType operator+(const _vectorNumType& other) const;
    _vectorNumType operator-(const _vectorNumType& other) const;
    _vectorNumType operator*(const _vectorNumType& other) const;
    _vectorNumType operator/(const _vectorNumType& other) const;
    _vectorNumType& operator+=(const _vectorNumType& other);
    _vectorNumType& operator-=(const _vectorNumType& other);
    _vectorNumType& operator*=(const _vectorNumType& other);
    _vectorNumType& operator/=(const _vectorNumType& other);
};

// 允许浮点数左边直接操作
inline _vectorNumType operator+(double f, const _vectorNumType& v) {
    return f + v.value;
}
inline _vectorNumType operator-(double f, const _vectorNumType& v) {
    return f - v.value;
}
inline _vectorNumType operator*(double f, const _vectorNumType& v) {
    return f * v.value;
}
inline _vectorNumType operator/(double f, const _vectorNumType& v) {
    assert(v != 0);
    return f / v.value;
}
inline _vectorNumType max(const _vectorNumType& a, const _vectorNumType& b) {
    return a > b ? a : b;
}
inline _vectorNumType min(const _vectorNumType& a, const _vectorNumType& b) {
    return a < b ? a : b;
}
inline void swap(_vectorNumType& a, _vectorNumType& b) {
    _vectorNumType tmp = a;
    a = b;
    b = tmp;
}

inline _vectorNumType sqrt(const _vectorNumType& x) {
    return sqrt(x.value);
}
inline _vectorNumType fabs(const _vectorNumType& x) {
    return fabs(x.value);
}
inline _vectorNumType atan2(const _vectorNumType& y, const _vectorNumType& x) {
    return atan2(y.value, x.value);
}
inline _vectorNumType asin(const _vectorNumType& x) {
    return asin(x.value);
}
inline _vectorNumType acos(const _vectorNumType& x) {
    return acos(x.value);
}
inline _vectorNumType cos(const _vectorNumType& x) {
    return cos(x.value);
}
inline _vectorNumType sin(const _vectorNumType& x) {
    return sin(x.value);
}
inline _vectorNumType tan(const _vectorNumType& x) {
    return tan(x.value);
}

inline std::ostream& operator<<(std::ostream& os, const _vectorNumType& v) {
    os << v.value;
    return os;
}

#endif