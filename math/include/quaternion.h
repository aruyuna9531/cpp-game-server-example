#ifndef QUATERNION_H_
#define QUATERNION_H_
#include "vector_num_type.h"
class Vector3;
class EulerAngles;

class Quaternion {
public:
    _vectorNumType x,y,z,w;
    void identity() {
        w = 1.0;
        x = y = z = 0.0;
    }

    // 叉乘
    Quaternion operator*(const Quaternion& a) const;
    Quaternion& operator*=(const Quaternion& a);

    // 正则化
    void normalize();

    // 幂
    Quaternion pow(const _vectorNumType& exponent);

    // 共轭
    Quaternion conjugate() const;
    
    // 获得旋转角&旋转轴
    _vectorNumType getRotationAngle() const;
    Vector3 getRotationAxis() const;

    // 旋转四元数
    void setToRotateAboutX(const _vectorNumType& theta);
    void setToRotateAboutY(const _vectorNumType& theta);
    void setToRotateAboutZ(const _vectorNumType& theta);
    void setToRotateAboutAxis(const Vector3& axis, const _vectorNumType& theta);

    // 构造执行物体-惯性旋转的四元数
    void setToRotateObjectToInertial(const EulerAngles& orientation);
    void setToRotateInertialToObject(const EulerAngles& orientation);
};

extern const Quaternion kQuaternionIdentity;

// 点乘
extern _vectorNumType dotProduct(const Quaternion& a, const Quaternion& b);

// 球面线性插值slerp
extern Quaternion slerp(const Quaternion& p, const Quaternion& q, const _vectorNumType& t);

#endif