#include "quaternion.h"
#include "math_util.h"
#include "vector3.h"
#include "euler_angles.h"
#include <assert.h>
#include <cmath>
#include "vector_num_type.h"

const Quaternion kQuaternionIdentity = {0.0,0.0,0.0,1.0};

// 绕x旋转 w=cos(θ/2)，x=sin(θ/2)
void Quaternion::setToRotateAboutX(const _vectorNumType& theta) {
    w = cos(theta * 0.5);
    x = sin(theta * 0.5);
    y = 0.0;
    z = 0.0;
}

// 绕y旋转 y=sin(θ/2)
void Quaternion::setToRotateAboutY(const _vectorNumType& theta) {
    w = cos(theta * 0.5);
    x = 0.0;
    y = sin(theta * 0.5);
    z = 0.0;
}

// 绕z旋转 z=sin(θ/2)
void Quaternion::setToRotateAboutZ(const _vectorNumType& theta) {
    w = cos(theta * 0.5);
    x = 0.0;
    y = 0.0;
    z = sin(theta * 0.5);
}

// 绕axis旋转
void Quaternion::setToRotateAboutAxis(const Vector3& axis, const _vectorNumType& theta) {
    assert(fabs(axis.Mag() - 1.0) == 0);
    _vectorNumType sinThetaOver2 = sin(theta * 0.5);
    w = cos(theta * 0.5);
    x = axis.x * sinThetaOver2;
    y = axis.y * sinThetaOver2;
    z = axis.z * sinThetaOver2;
}

// 物体-惯性旋转的四元数 10.6.5
void Quaternion::setToRotateObjectToInertial(const EulerAngles& orientation) {
    _vectorNumType sp = sin(orientation.pitch * 0.5);   // sin(pitch/2)
    _vectorNumType sb = sin(orientation.bank * 0.5);   // sin(bank/2)
    _vectorNumType sh = sin(orientation.heading * 0.5);   // sin(heading/2)
    _vectorNumType cp = cos(orientation.pitch * 0.5);   // cos(pitch/2)
    _vectorNumType cb = cos(orientation.bank * 0.5);   // cos(bank/2)
    _vectorNumType ch = cos(orientation.heading * 0.5);   // cos(heading/2)
    w = ch*cp*cb+sh*sp*sb;
    x = ch*sp*cb+sh*cp*sb;
    y = -ch*sp*sb+sh*cp*cb;
    z = -sh*sp*cb+ch*cp*sb;
}

// 惯性-物体旋转的四元数 10.6.5
void Quaternion::setToRotateInertialToObject(const EulerAngles& orientation) {
    _vectorNumType sp = sin(orientation.pitch * 0.5);   // sin(pitch/2)
    _vectorNumType sb = sin(orientation.bank * 0.5);   // sin(bank/2)
    _vectorNumType sh = sin(orientation.heading * 0.5);   // sin(heading/2)
    _vectorNumType cp = cos(orientation.pitch * 0.5);   // cos(pitch/2)
    _vectorNumType cb = cos(orientation.bank * 0.5);   // cos(bank/2)
    _vectorNumType ch = cos(orientation.heading * 0.5);   // cos(heading/2)
    w = ch*cp*cb+sh*sp*sb;
    x = -ch*sp*cb-sh*cp*sb;
    y = ch*sp*sb-sh*cb*cp;
    z = sh*sp*cb-ch*cp*sb;
}

// 四元数叉乘（*注：和标准定义是相反的）
Quaternion Quaternion::operator*(const Quaternion& a) const {
    Quaternion result;
    result.w = w*a.w - x*a.x - y*a.y - z*a.z;
    result.x = w*a.x + x*a.w + z*a.y - y*a.z;
    result.y = w*a.y + y*a.w + x*a.z - z*a.x;
    result.z = w*a.z + z*a.w + y*a.x - x*a.y;
    return result;
}

Quaternion& Quaternion::operator*=(const Quaternion& a) {
    *this = *this * a;
    return *this;
}

void Quaternion::normalize() {
#ifdef RELEASE_
    identity();
    return;
#endif
    _vectorNumType mag = sqrt(w*w + x*x + y*y + z*z);
    assert(mag > 0);
    _vectorNumType overmag = 1.0 / mag;
    w *= overmag;
    x *= overmag;
    y *= overmag;
    z *= overmag;
}

// 旋转角
_vectorNumType Quaternion::getRotationAngle() const {
    return safeACos(w) * 2.0;
}

// 旋转轴
Vector3 Quaternion::getRotationAxis() const {
    _vectorNumType sinThetaOver2Sq = 1.0 - w*w;     // w = cos(θ/2)， sin^2(θ/2) = 1-cos^2(θ/2) = 1-w^2，sin(θ/2) = sqrt(1-w^2)
    if (sinThetaOver2Sq <= 0) {
        return Vector3(1,0,0);
    }
    _vectorNumType oneOverSinThetaOver2 = 1 / sqrt(sinThetaOver2Sq);    // 1 / sin(θ/2)
    return Vector3(
        x*oneOverSinThetaOver2,
        y*oneOverSinThetaOver2,
        z*oneOverSinThetaOver2
    );  // (x/sin(θ/2), y/sin(θ/2), z/sin(θ/2))
}

_vectorNumType dotProduct(const Quaternion& a, const Quaternion& b) {
    return a.w*b.w+a.x*b.x+a.y*b.y+a.z*b.z;
}

Quaternion slerp(const Quaternion& q0, const Quaternion& q1, const _vectorNumType& t) {
    if (t <= 0.0) return q0;
    if (t >= 1.0) return q1;
    _vectorNumType cosOmega = dotProduct(q0, q1);   // cosΩ

    _vectorNumType q1w = q1.w;
    _vectorNumType q1x = q1.x;
    _vectorNumType q1y = q1.y;
    _vectorNumType q1z = q1.z;
    if (cosOmega < 0.0) {
        // 点乘为负使用-q1。q和-q代表相同的旋转，但可能产生不同的slerp运算，这里取负是方便用锐角进行运算。
        q1w = -q1w;
        q1x = -q1x;
        q1y = -q1y;
        q1z = -q1z;
        cosOmega = -cosOmega;
    }
    assert(cosOmega <= 1.0);
    _vectorNumType k0, k1;  // 插值片
    if (cosOmega > 1.0 - 1e-5) {
        // 非常接近的情况，按线性插值处理
        k0 = 1.0 - t;
        k1 = t;
    } else {
        _vectorNumType sinOmega = sqrt(1.0 - cosOmega * cosOmega);  // sin^2 + cos^2 = 1
        _vectorNumType omega = atan2(sinOmega, cosOmega);
        _vectorNumType oneOverSinOmega = 1 / sinOmega;
        k0 = sin((1-t)*omega) * oneOverSinOmega;    // sin((1-t)Ω) / sinΩ
        k1 = sin(t*omega) * oneOverSinOmega;        // sin(tΩ) / sinΩ
    }

    Quaternion result;
    result.x = k0*q0.x + k1*q1x;
    result.y = k0*q0.y + k1*q1y;
    result.z = k0*q0.z + k1*q1z;
    result.w = k0*q0.w + k1*q1w;
    return result;
}

// 共轭四元数 [w -q]
Quaternion Quaternion::conjugate() const {
    Quaternion result;
    result.w = w;
    result.x = -x;
    result.y = -y;
    result.z = -z;
    return result;
}

Quaternion Quaternion::pow(const _vectorNumType& exponent) {
    if (fabs(w) > 1 - 1e-4) {
        // 单位向量 无需计算
        return *this;
    }

    _vectorNumType alpha = acos(w);
    _vectorNumType newAlpha = alpha * exponent;
    _vectorNumType mult = sin(newAlpha) / sin(alpha);
    Quaternion result;
    result.w = cos(newAlpha);
    result.x = x * mult;
    result.y = y * mult;
    result.z = z * mult;
    return result;
}