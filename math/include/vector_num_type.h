
// 方便浮点数==用的 注意会带来的精度丢失问题
#ifndef VECTOR_NUM_TYPE_H_
#define VECTOR_NUM_TYPE_H_

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
extern _vectorNumType operator+(double f, const _vectorNumType& v);
extern _vectorNumType operator-(double f, const _vectorNumType& v);
extern _vectorNumType operator*(double f, const _vectorNumType& v);
extern _vectorNumType operator/(double f, const _vectorNumType& v);

extern _vectorNumType max(const _vectorNumType& a, const _vectorNumType& b);
extern _vectorNumType min(const _vectorNumType& a, const _vectorNumType& b);
extern void swap(_vectorNumType& a, _vectorNumType& b);

extern _vectorNumType sqrt(const _vectorNumType& x);
extern _vectorNumType fabs(const _vectorNumType& x);
extern _vectorNumType atan2(const _vectorNumType& y, const _vectorNumType& x);
extern _vectorNumType asin(const _vectorNumType& x);
extern _vectorNumType acos(const _vectorNumType& x);
extern _vectorNumType cos(const _vectorNumType& x);
extern _vectorNumType sin(const _vectorNumType& x);
extern _vectorNumType tan(const _vectorNumType& x);

#endif