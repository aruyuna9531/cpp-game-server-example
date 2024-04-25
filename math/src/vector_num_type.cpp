#include "vector_num_type.h"
#include <cmath>
#include <assert.h>

_vectorNumType::_vectorNumType(){
    value = 0;
}
_vectorNumType::_vectorNumType(const double& _v) {
    value = _v;
}
_vectorNumType& _vectorNumType::operator=(const double& _v) {
    value = _v;
    return *this;
}
// 差值误差在这个范围内就认为相等
bool _vectorNumType::operator==(const _vectorNumType& other) const {
    return value-other.value < 1e-9 && value-other.value > -1e-9;
}
bool _vectorNumType::operator>(const _vectorNumType& other) const {
    return value-other.value >= 1e-9;
}
bool _vectorNumType::operator<(const _vectorNumType& other) const {
    return value-other.value <= -1e-9;
}
bool _vectorNumType::operator!=(const _vectorNumType& other) const {
    return *this > other || *this < other;
}
bool _vectorNumType::operator>=(const _vectorNumType& other) const {
    return *this > other || *this == other;
}
bool _vectorNumType::operator<=(const _vectorNumType& other) const {
    return *this < other || *this == other;
}

_vectorNumType _vectorNumType::operator-() const {
    return _vectorNumType(-value);
}
_vectorNumType _vectorNumType::operator+(const _vectorNumType& other) const {
    return _vectorNumType(value + other.value);
}
_vectorNumType _vectorNumType::operator-(const _vectorNumType& other) const {
    return _vectorNumType(value - other.value);
}
_vectorNumType _vectorNumType::operator*(const _vectorNumType& other) const {
    return _vectorNumType(value * other.value);
}
_vectorNumType _vectorNumType::operator/(const _vectorNumType& other) const {
    return _vectorNumType(value / other.value);
}
_vectorNumType& _vectorNumType::operator+=(const _vectorNumType& other) {
    value += other.value;
    return *this;
}
_vectorNumType& _vectorNumType::operator-=(const _vectorNumType& other) {
    value -= other.value;
    return *this;
}
_vectorNumType& _vectorNumType::operator*=(const _vectorNumType& other) {
    value *= other.value;
    return *this;
}
_vectorNumType& _vectorNumType::operator/=(const _vectorNumType& other) {
    value /= other.value;
    return *this;
}

_vectorNumType sqrt(const _vectorNumType& x) {
    return sqrt(x.value);
}

_vectorNumType fabs(const _vectorNumType& x) {
    return fabs(x.value);
}

_vectorNumType atan2(const _vectorNumType& y, const _vectorNumType& x) {
    return atan2(y.value, x.value);
}

_vectorNumType asin(const _vectorNumType& x) {
    return asin(x.value);
}
_vectorNumType acos(const _vectorNumType& x) {
    return acos(x.value);
}
_vectorNumType cos(const _vectorNumType& x) {
    return cos(x.value);
}
_vectorNumType sin(const _vectorNumType& x) {
    return sin(x.value);
}
_vectorNumType tan(const _vectorNumType& x) {
    return tan(x.value);
}
_vectorNumType operator+(double f, const _vectorNumType& v) {
    return f + v.value;
}
_vectorNumType operator-(double f, const _vectorNumType& v) {
    return f - v.value;
}
_vectorNumType operator*(double f, const _vectorNumType& v) {
    return f * v.value;
}
_vectorNumType operator/(double f, const _vectorNumType& v) {
    assert(v != 0);
    return f / v.value;
}