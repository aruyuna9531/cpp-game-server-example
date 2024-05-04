#include <assert.h>
#include <math.h>
#include "vector3.h"
#include "euler_angles.h"
#include "quaternion.h"
#include "rotation_matrix.h"
#include "matrix4x3.h"
#include "math_util.h"
#include "vector_num_type.h"

void Matrix4x3::identity() {
    m11 = 1; m12 = 0; m13 = 0;
    m21 = 0; m22 = 1; m23 = 0;
    m31 = 0; m32 = 0; m33 = 1;
    tx = 0; ty = 0; tz = 1;
}

// 不平移
void Matrix4x3::zeroTranslation() {
    tx = ty = tz = 0;
}

// 设置平移部分
void Matrix4x3::setTranslation(const Vector3& d) {
    tx = d.x;
    ty = d.y;
    tz = d.z;
}

// 只平移
void Matrix4x3::setupTranslation(const Vector3& d) {
    m11 = 1; m12 = 0; m13 = 0;
    m21 = 0; m22 = 1; m23 = 0;
    m31 = 0; m32 = 0; m33 = 1; 
    tx = d.x; ty = d.y; tz = d.z;
}

// 执行局部→父空间变换的矩阵，局部空间的位置和方位在父空间描述
// 常见用途是构造物体→世界的变换矩阵，中间会变到惯性空间
void Matrix4x3::setupLocalToParent(const Vector3& pos , const EulerAngles& orient) {
    // 构造旋转矩阵
    RotationMatrix orientMatrix;
    orientMatrix.setup(orient);
    setupLocalToParent(pos, orientMatrix);
}

void Matrix4x3::setupLocalToParent(const Vector3& pos , const RotationMatrix& orient) {
    // SRT原则的R（旋转）部分，是orient的转置（旋转矩阵一般是惯性→物体（父→局），现在要求的是局→父，要做转置）
    m11 = orient.m11; m12 = orient.m21; m13 = orient.m31;
    m21 = orient.m12; m22 = orient.m22; m23 = orient.m32;
    m31 = orient.m13; m32 = orient.m23; m33 = orient.m33;
    // 平移部分直接复制过来就行
    tx = pos.x; ty = pos.y; tz = pos.z;
}

// 执行父空间→局部变换的矩阵，局部空间的位置和方位在父空间描述
// 常见用途是构造世界→物体的变换矩阵，通常中间也会先转到惯性空间。
void Matrix4x3::setupParentToLocal(const Vector3& pos , const EulerAngles& orient) {
    // 构造旋转矩阵
    RotationMatrix orientMatrix;
    orientMatrix.setup(orient);
    setupParentToLocal(pos, orientMatrix);
}

void Matrix4x3::setupParentToLocal(const Vector3& pos , const RotationMatrix& orient) {
    // 旋转部分，这里不用转置
    m11 = orient.m11; m12 = orient.m12; m13 = orient.m13;
    m21 = orient.m21; m22 = orient.m22; m23 = orient.m23;
    m31 = orient.m31; m32 = orient.m32; m33 = orient.m33;
    // 旋转是先发生的，要先旋转平移部分。-pos × m
    tx = -(pos.x * m11 + pos.y * m21 + pos.z * m31);
    ty = -(pos.x * m12 + pos.y * m22 + pos.z * m32);
    tz = -(pos.x * m13 + pos.y * m23 + pos.z * m33);
}

// 绕坐标轴旋转的矩阵，左手原则，平移部分为0 8.2.2
// 旋转轴：1-x，2-y，3-z
void Matrix4x3::setupRotate(int axis, const _vectorNumType& theta) {
    _vectorNumType s = sin(theta);
    _vectorNumType c = cos(theta);
    switch(axis) {
        case 1:
            // x轴：
            // 1    0       0
            // 0    cosθ    sinθ
            // 0    -sinθ   cosθ
            m11 = 1; m12 = 0; m13 = 0;
            m21 = 0; m22 = c; m23 = s;
            m31 = 0; m32 = -s; m33 = c;
            break;
        case 2:
            // y轴：
            // cosθ 0       -sinθ
            // 0    1       0
            // sinθ 0       cosθ
            m11 = c; m12 = 0; m13 = -s;
            m21 = 0; m22 = 1; m23 = 0;
            m31 = s; m32 = 0; m33 = c;
            break;
        case 3:
            // z轴：
            // cosθ     sinθ    0
            // -sinθ    cosθ    0
            // 0        0       1
            m11 = c; m12 = s; m13 = 0;
            m21 = -s; m22 = c; m23 = 0;
            m31 = 0; m32 = 0; m33 = 1;
            break;
        default:
            assert(false);
    }
    tx = ty = tz = 0;
}

// 绕任意轴旋转。旋转轴通过原点并且为单位向量 8.2.3
void Matrix4x3::setupRotate(const Vector3& axis, const _vectorNumType& theta) {
    assert(axis * axis == 1);
    _vectorNumType s = sin(theta);
    _vectorNumType c = cos(theta);

    _vectorNumType a = 1 - c;
    _vectorNumType ax = a * axis.x;
    _vectorNumType ay = a * axis.y;
    _vectorNumType az = a * axis.z;

    m11 = ax * axis.x + c;
    m12 = ax * axis.y + axis.z * s;
    m13 = ax * axis.z - axis.y * s;

    m21 = ay * axis.x - axis.z * s;
    m22 = ay * axis.y + c;
    m23 = ay * axis.z + axis.x * s;

    m31 = az * axis.x + axis.y * s;
    m32 = az * axis.y - axis.x * s;
    m33 = az * axis.z + c;

    tx = ty = tz = 0;
}

// 四元数转换到矩阵 10.6.3
void Matrix4x3::fromQuaternion(const Quaternion& q) {
    _vectorNumType ww = 2 * q.w;
    _vectorNumType xx = 2 * q.x;
    _vectorNumType yy = 2 * q.y;
    _vectorNumType zz = 2 * q.z;

    m11 = 1 - yy * q.y - zz * q.z;
    m12 = xx * q.y + ww * q.z;
    m13 = xx * q.z - ww * q.x;

    m21 = xx * q.y - ww * q.z;
    m22 = 1 - xx * q.x - zz * q.z;
    m23 = yy * q.z + ww * q.x;

    m31 = xx * q.z + ww * q.y;
    m32 = yy * q.z - ww * q.x;
    m33 = 1 - xx * q.x - yy * q.y;

    tx = ty = tz = 0;
}

// 沿坐标轴缩放矩阵，缩放轴为(k,k,k) 8.3.1
void Matrix4x3::setupScale(const Vector3& s) {
    m11 = s.x; m12 = 0; m13 = 0;
    m21 = 0; m22 = s.y; m23 = 0;
    m31 = 0; m32 = 0; m33 = s.z;
    tx = ty = tz = 0;
}

// 任意轴缩放，旋转轴为单位向量 8.3.2
void Matrix4x3::setupScaleAlongAxis(const Vector3& axis, const _vectorNumType& k) {
    assert(axis * axis == 1);

    _vectorNumType a = k - 1;
    _vectorNumType ax = a * axis.x;
    _vectorNumType ay = a * axis.y;
    _vectorNumType az = a * axis.z;

    m11 = ax * axis.x + 1;
    m22 = ay * axis.y + 1;
    m33 = az * axis.z + 1;
    m12 = m21 = ax * axis.y;
    m13 = m31 = ax * axis.z;
    m23 = m32 = ay * axis.z;
    tx = ty = tz = 0;
}

// 切变矩阵 8.6
// axis = 1 (x轴) → y += s*x, z += t*x
// axis = 2 (y轴) → x += s*y, z += t*y
// axis = 3 (z轴) → x += s*z, y += t*z
void Matrix4x3::setupShear(int axis, const _vectorNumType& s, const _vectorNumType& t) {
    switch (axis) {
        case 1:
            // 1 s t
            // 0 1 0
            // 0 0 1
            m11 = 1; m12 = s; m13 = t;
            m21 = 0; m22 = 1; m23 = 0;
            m31 = 0; m32 = 0; m33 = 1;
            break;
        case 2:
            // 1 0 0
            // s 1 t
            // 0 0 1
            m11 = 1; m12 = 0; m13 = 0;
            m21 = s; m22 = 1; m23 = t;
            m31 = 0; m32 = 0; m33 = 1;
            break;
        case 3:
            // 1 0 0
            // 0 1 0
            // s t 1
            m11 = 1; m12 = 0; m13 = 0;
            m21 = 0; m22 = 1; m23 = 0;
            m31 = s; m32 = t; m33 = 1;
            break;
        default:
            assert(false);
    }
    tx = ty = tz = 0;
}

// 投影矩阵，平面过原点，垂直于单位向量n 8.4.2
void Matrix4x3::setupProject(const Vector3& n) {
    assert(n * n == 1);

    m11 = 1 - n.x * n.x;
    m22 = 1 - n.y * n.y;
    m33 = 1 - n.z * n.z;

    m12 = m21 = -n.x * n.y;
    m13 = m31 = -n.x * n.z;
    m23 = m32 = -n.y * n.z;

    tx = ty = tz = 0;
}

// 反射矩阵，反射平面平行于坐标平面。8.5
// axis = 1 沿x=k 面反射
// axis = 2 沿y=k 面反射
// axis = 3 沿z=k 面反射
void Matrix4x3::setupReflect(int axis, const _vectorNumType& k) {
    switch(axis) {
        case 1:
            m11 = -1; m12 = 0; m13 = 0;
            m21 = 0; m22 = 1; m23 = 0;
            m31 = 0; m32 = 0; m33 = 1;
            tx = 2 * k;
            ty = tz = 0;
            break;
        case 2:
            m11 = 1; m12 = 0; m13 = 0;
            m21 = 0; m22 = -1; m23 = 0;
            m31 = 0; m32 = 0; m33 = 1;
            ty = 2 * k;
            tx = tz = 0;
            break;
        case 3:
            m11 = 1; m12 = 0; m13 = 0;
            m21 = 0; m22 = 1; m23 = 0;
            m31 = 0; m32 = 0; m33 = -1;
            tz = 2 * k;
            tx = ty = 0;
            break;
        default:
            assert(false);
    }
}

// 构造反射矩阵，反射平面为通过原点的任意平面，垂直于单位向量n 8.5
void Matrix4x3::setupReflect(const Vector3& n) {
    assert(n * n == 1);

    _vectorNumType ax = -2 * n.x;
    _vectorNumType ay = -2 * n.y;
    _vectorNumType az = -2 * n.z;

    m11 = 1 + ax * n.x;
    m22 = 1 + ay * n.y;
    m33 = 1 + az * n.z;

    m12 = m21 = ax * n.y;
    m13 = m31 = ax * n.z;
    m23 = m32 = ay * n.z;

    tx = ty = tz = 0;
}

// 沿m变换点p 7.1.7
Vector3 operator*(const Vector3& p, const Matrix4x3& m) {
    return Vector3 (
        p.x * m.m11 + p.y * m.m21 + p.z * m.m31 + m.tx,
        p.x * m.m12 + p.y * m.m22 + p.z * m.m32 + m.ty,
        p.x * m.m13 + p.y * m.m23 + p.z * m.m33 + m.tz
    );
}

Vector3& operator*=(Vector3& p, const Matrix4x3& m) {
    p = p * m;
    return p;
}

// 矩阵×矩阵（补充4阶） 7.1.6
Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b) {
    Matrix4x3 r;
    r.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
    r.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
    r.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;
    
    r.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
    r.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
    r.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;

    r.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
    r.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
    r.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;

    r.tx = a.tx * b.m11 + a.ty * b.m21 + a.tz * b.m31 + b.tx;
    r.ty = a.tx * b.m12 + a.ty * b.m22 + a.tz * b.m32 + b.ty;
    r.tz = a.tx * b.m13 + a.ty * b.m23 + a.tz * b.m33 + b.tz;

    return r;
}

Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b) {
    a = a * b;
    return a;    
}

// |m| 9.1.1
_vectorNumType determinant(const Matrix4x3& m) {
    return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) + m.m12 * (m.m23 * m.m31 - m.m21 * m.m33) + m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// M^-1 9.2.1
Matrix4x3 inverse(const Matrix4x3& m) {
    _vectorNumType det = determinant(m);
    assert(det != 0);
    _vectorNumType oneOverDet = 1 / det;
    Matrix4x3 r;

    r.m11 = (m.m22 * m.m33 - m.m23 * m.m32) * oneOverDet;
    r.m12 = (m.m13 * m.m32 - m.m12 * m.m33) * oneOverDet;
    r.m13 = (m.m12 * m.m23 - m.m13 * m.m22) * oneOverDet;
    
    r.m21 = (m.m23 * m.m31 - m.m21 * m.m33) * oneOverDet;
    r.m22 = (m.m11 * m.m33 - m.m13 * m.m31) * oneOverDet;
    r.m23 = (m.m13 * m.m21 - m.m11 * m.m23) * oneOverDet;
    
    r.m31 = (m.m21 * m.m32 - m.m22 * m.m31) * oneOverDet;
    r.m32 = (m.m12 * m.m31 - m.m11 * m.m32) * oneOverDet;
    r.m33 = (m.m11 * m.m22 - m.m12 * m.m21) * oneOverDet;

    r.tx = -(m.tx * r.m11 + m.ty * r.m21 + m.tz * r.m31);
    r.ty = -(m.tx * r.m12 + m.ty * r.m22 + m.tz * r.m32);
    r.tz = -(m.tx * r.m13 + m.ty * r.m23 + m.tz * r.m33);

    return r;
}

Vector3 getTranslation(const Matrix4x3& m) {
    return Vector3(m.tx, m.ty, m.tz);
}

// 世界→物体 变换矩阵中提取物体的位置
// -t × m^T
Vector3 getPositionFromParentToLocalMatrix(const Matrix4x3& m) {
    return Vector3(
        -(m.tx * m.m11 + m.ty * m.m12 + m.tz * m.m13),
        -(m.tx * m.m21 + m.ty * m.m22 + m.tz * m.m23),
        -(m.tx * m.m31 + m.ty * m.m32 + m.tz * m.m33)
    );
}

// 物体→世界 提取物体的位置（就是getTranslation）
Vector3 getPositionFromLocalToParentMatrix(const Matrix4x3& m) {
    return Vector3(m.tx, m.ty, m.tz);
}
