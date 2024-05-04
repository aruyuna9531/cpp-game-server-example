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