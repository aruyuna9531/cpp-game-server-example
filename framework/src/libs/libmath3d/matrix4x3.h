#ifndef MATRIX4x3_H_
#define MATRIX4x3_H_
#include "vector_num_type.h"
class Vector3;
class EulerAngles;
class Quaternion;
class RotationMatrix;

// 一个4×3矩阵表示线性变换+平移，在进行矩阵计算时，默认加上第4列[0,0,0,1]^T构成4阶矩阵进行计算
// 涉及3维向量和4×3矩阵的计算，会将3维向量加上第4维（1）作为4维向量与这个4维矩阵计算
class Matrix4x3 {
public:
    // 线性变换部分
    _vectorNumType m11, m12, m13;
    _vectorNumType m21, m22, m23;
    _vectorNumType m31, m32, m33;
    // 平移部分
    _vectorNumType tx, ty, tz;

    void identity();

    // 平移部分相关
    void zeroTranslation();
    void setTranslation(const Vector3& d);
    void setupTranslation(const Vector3& d);

    // 父空间与局部空间互相变换。假定局部空间在指定的位置和方位，可能用欧拉角或旋转矩阵表示
    void setupLocalToParent(const Vector3& pos, const EulerAngles& orient);
    void setupLocalToParent(const Vector3& pos, const RotationMatrix& orient);
    void setupParentToLocal(const Vector3& pos, const EulerAngles& orient);
    void setupParentToLocal(const Vector3& pos, const RotationMatrix& orient);

    void setupRotate(int axis, const _vectorNumType& theta); // 构造绕axis坐标轴旋转θ的矩阵
    void setupRotate(const Vector3& axis, const _vectorNumType& theta); // 构造绕任意轴旋转的矩阵
    void fromQuaternion(const Quaternion& q);   // 构造旋转矩阵，角位移有四元数给出
    void setupScale(const Vector3& s);  // 构造沿坐标轴缩放的矩阵
    void setupScaleAlongAxis(const Vector3& axis, const _vectorNumType& k); // 构造焰任意轴缩放的矩阵
    void setupShear(int axis, const _vectorNumType& s, const _vectorNumType& t);    // 构造切变矩阵
    void setupProject(const Vector3& n);    // 构造投影矩阵（投影平面过原点）
    void setupReflect(int axis, const _vectorNumType& k = 0.0); // 构造反射矩阵
    void setupReflect(const Vector3& n);    // 构造沿任意平面反射的矩阵
};

Vector3 operator*(const Vector3& p, const Matrix4x3& m);
Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b);

Vector3& operator*=(Vector3& p, const Matrix4x3& m);
Matrix4x3& operator*=(const Matrix4x3& a, const Matrix4x3& b);

_vectorNumType determinant(const Matrix4x3& m); // 3×3部分的行列式
Matrix4x3 inverse(const Matrix4x3& m);  // 矩阵的逆
Vector3 getTranslation(const Matrix4x3& m);     // 平移部分

// 从局部矩阵→父矩阵（或反过来）取位置/方位
Vector3 getPositionFromParentToLocalMatrix(const Matrix4x3& m);
Vector3 getPositionFromLocalToParentMatrix(const Matrix4x3& m);
#endif