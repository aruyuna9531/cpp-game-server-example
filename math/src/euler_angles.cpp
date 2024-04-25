#include <cmath>
#include "euler_angles.h"
#include "quaternion.h"
#include "math_util.h"
#include "matrix4x3.h"
#include "rotation_matrix.h"

const EulerAngles kEulerAnglesIdentity(0,0,0);

EulerAngles::EulerAngles(const _vectorNumType& h, const _vectorNumType& p, const _vectorNumType& b): heading(h), pitch(p), bank(b){}

void EulerAngles::canonize() {
    pitch = wrapPi(pitch);  // pitch → [-π, π]
    if (pitch < -pi/2.0) {
        pitch = -pi - pitch;
        heading += pi;
        bank += pi;
    } else if (pitch > pi/2.0) {
        pitch = pi - pitch;
        heading += pi;
        bank += pi;
    }

    if (fabs(pitch) > pi/2.0 - 1e-4) {
        // 万向锁 绕垂直轴的旋转付给heading
        heading += bank;
        bank = 0;
    } else {
        // 非万向锁
        bank = wrapPi(bank);
    }

    heading = wrapPi(heading);
}

// 物体→惯性四元数 10.6.6
void EulerAngles::fromObjectToInertialQuaternion(const Quaternion& q) {
    _vectorNumType sp = -2.0 * (q.y*q.z - q.w*q.x);     // sin(pitch)
    if (fabs(sp) > 1.0 - 1e-4) {
        pitch = pi/2.0 * sp;
        // bank置0，计算heading
        heading = atan2(-q.x*q.z + q.w*q.y, 0.5 - q.y*q.y - q.z*q.z);
        bank = 0;
    } else {
        // 计算角度
        pitch = asin(sp);
        heading = atan2(q.x*q.z + q.w*q.y, 0.5 - q.x*q.x - q.y*q.y);
        bank = atan2(q.x*q.y + q.w*q.z, 0.5 - q.x*q.x - q.z*q.z);
    }
}

void EulerAngles::fromInertialToObjectQuaternion(const Quaternion& q) {
    // 跟obj→ine的区别：3个atan2的y不一样
    _vectorNumType sp = -2.0 * (q.y*q.z - q.w*q.x);
    if (fabs(sp) > 1.0 - 1e-4) {
        pitch = pi/2.0 * sp;
        heading = atan2(-q.x*q.z - q.w*q.y, 0.5 - q.y*q.y - q.z*q.z);
        bank = 0;
    } else {
        // 计算角度
        pitch = asin(sp);
        heading = atan2(q.x*q.z - q.w*q.y, 0.5 - q.x*q.x - q.y*q.y);
        bank = atan2(q.x*q.y - q.w*q.z, 0.5 - q.x*q.x - q.z*q.z);
    }
}

// 物体-世界坐标系变换矩阵到欧拉角，假设矩阵正交（忽略平移）10.6.2
void EulerAngles::fromObjectToWorldMatrix(const Matrix4x3& m) {
    _vectorNumType sp = -m.m32;
    if (fabs(sp) > 10.0 - 1e-5) {
        pitch = pi/2.0 * sp;
        heading = atan2(-m.m13, m.m11);     // -m.m23？
        bank = 0.0;
    } else {
        heading = atan2(m.m31, m.m33);
        pitch = asin(sp);
        bank = atan2(m.m12, m.m22);
    }
}

// 世界-物体坐标系变换矩阵到欧拉角，假设矩阵正交（忽略平移）10.6.2
void EulerAngles::fromWorldToObjectMatrix(const Matrix4x3& m) {
    _vectorNumType sp = -m.m23;
    if (fabs(sp) > 10.0 - 1e-5) {
        pitch = pi/2.0 * sp;
        heading = atan2(-m.m31, m.m11);     // -m.m32？
        bank = 0.0;
    } else {
        heading = atan2(m.m13, m.m33);
        pitch = asin(sp);
        bank = atan2(m.m21, m.m22);
    }
}

// 旋转矩阵→欧拉角
void EulerAngles::fromRotationMatrix(const RotationMatrix& m) {
    _vectorNumType sp = -m.m23;
    if (fabs(sp) > 10 - 1e-5) {
        pitch = pi/2.0 * sp;
        heading = atan2(-m.m31, m.m11);
        bank = 0.0;
    } else {
        heading = atan2(m.m13, m.m33);
        pitch = asin(sp);
        bank = atan2(m.m21, m.m22);
    }
}