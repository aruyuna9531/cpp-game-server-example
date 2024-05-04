#include "vector3.h"
#include "math_util.h"
#include "quaternion.h"
#include "euler_angles.h"
#include "rotation_matrix.h"

// 惯性到物体的变换：i × m = o
//            | m11 m12 m13 |
// [ix iy iz] | m21 m22 m23 | = [ox oy oz]
//            | m31 m32 m33 |
// 物体到惯性的变换：o × m^T = i
//            | m11 m21 m31 |
// [ox oy oz] | m12 m22 m32 | = [ox oy oz]
//            | m13 m23 m33 |
// 或者这么表示，好看点：
// 惯性到物体的变换：m^T × i^T = o^T
// | m11 m21 m31 | | ix |   | ox |
// | m12 m22 m32 | | iy | = | oy |
// | m13 m23 m33 | | iz |   | oz |
// 物体到惯性的变换：m × o^T = i^T
// | m11 m12 m13 | | ox |   | ix |
// | m21 m22 m23 | | oy | = | iy |
// | m31 m32 m33 | | oz |   | iz |

void RotationMatrix::identity() {
    m11 = 1; m12 = 0; m13 = 0;
    m21 = 0; m22 = 1; m23 = 0;
    m31 = 0; m32 = 0; m33 = 1;
}

void RotationMatrix::setup(const EulerAngles& orientation) {
    _vectorNumType sh = sin(orientation.heading);
    _vectorNumType ch = cos(orientation.heading);
    _vectorNumType sp = sin(orientation.pitch);
    _vectorNumType cp = cos(orientation.pitch);
    _vectorNumType sb = sin(orientation.bank);
    _vectorNumType cb = cos(orientation.bank);

    m11 = ch * cb + sh * sp * sb;
    m12 = -ch * sb + sh * sp * cb;
    m13 = sh * cp;

    m21 = sb * cp;
    m22 = cb * cp;
    m23 = -sp;

    m31 = -sh * cb + ch * sp * sb;
    m32 = sb * sh + ch * sp * cb;
    m33 = ch * cp;
}

// 惯性-物体旋转四元数
void RotationMatrix::fromInertialToObjectQuaternion(const Quaternion& q) {
    m11 = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    m12 = 2.0 * (q.x * q.y + q.w * q.z);
    m13 = 2.0 * (q.x * q.z - q.w * q.y);

    m21 = 2.0 * (q.x * q.y - q.w * q.z);
    m22 = 1.0 - 2.0 * (q.x * q.x + q.z * q.z);
    m23 = 2.0 * (q.y * q.z + q.w * q.x);

    m31 = 2.0 * (q.x * q.z + q.w * q.y);
    m32 = 2.0 * (q.y * q.z - q.w * q.x);
    m33 = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
}

// 物体-惯性旋转四元数
void RotationMatrix::fromObjectToInertialQuaternion(const Quaternion& q) {
    m11 = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    m12 = 2.0 * (q.x * q.y - q.w * q.z);
    m13 = 2.0 * (q.x * q.z + q.w * q.y);

    m21 = 2.0 * (q.x * q.y + q.w * q.z);
    m22 = 1.0 - 2.0 * (q.x * q.x + q.z * q.z);
    m23 = 2.0 * (q.y * q.z - q.w * q.x);

    m31 = 2.0 * (q.x * q.z - q.w * q.y);
    m32 = 2.0 * (q.y * q.z + q.w * q.x);
    m33 = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
}

// 向量的惯性-物体变换
Vector3 RotationMatrix::inertialToObject(const Vector3& v) const {
    // v × this
    return Vector3(
        m11*v.x + m21*v.y + m31*v.z,
        m12*v.x + m22*v.y + m32*v.z,
        m13*v.x + m23*v.y + m33*v.z
    );
}
// 向量的物体-惯性变换
Vector3 RotationMatrix::objectToInertial(const Vector3& v) const {
    // v × this^T
    return Vector3(
        m11*v.x + m12*v.y + m13*v.z,
        m21*v.x + m22*v.y + m23*v.z,
        m31*v.x + m32*v.y + m33*v.z
    );
}