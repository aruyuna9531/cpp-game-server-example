#ifndef ROTATION_MATRIX_H_
#define ROTATION_MATRIX_H_
#include "vector_num_type.h"
class Vector3;
class EulerAngles;
class Quaternion;

class RotationMatrix{
public:
    _vectorNumType m11, m12, m13;
    _vectorNumType m21, m22, m23;
    _vectorNumType m31, m32, m33;

    void identity();
    void setup(const EulerAngles& orientation); // 欧拉角构造矩阵

    void fromInertialToObjectQuaternion(const Quaternion& q);   // 四元数构造矩阵
    void fromObjectToInertialQuaternion(const Quaternion& q);

    Vector3 inertialToObject(const Vector3& v) const; // 旋转
    Vector3 objectToInertial(const Vector3& v) const;
};
#endif

