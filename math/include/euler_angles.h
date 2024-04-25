#ifndef EULER_ANGLES_H_
#define EULER_ANGLES_H_

#include "vector_num_type.h"
class Quaternion;
class Matrix4x3;
class RotationMatrix;

// 欧拉角
class EulerAngles {
public:
    // heading: 物体坐标系与惯性坐标系重合时，heading为绕y轴的旋转量（+z向+x轴为正向。参照左手坐标系，下同）
    _vectorNumType heading;
    // pitch: 经过heading旋转后，pitch为绕物体坐标系的x轴的旋转量（+y向+z轴方向为正）
    _vectorNumType pitch;
    // bank：经过pitch旋转后，bank为绕物体坐标系的z轴的旋转量（+x向+y轴方向为正）
    _vectorNumType bank;

    EulerAngles(){}
    EulerAngles(const _vectorNumType& h, const _vectorNumType& p, const _vectorNumType& b);
    // 置0
    void identity() {   
        heading = pitch = bank = 0;
    }
    // 限制集欧拉角
    void canonize();

    // 四元数→欧拉角 10.6.6
    void fromObjectToInertialQuaternion(const Quaternion &q);   // 物体-惯性
    void fromInertialToObjectQuaternion(const Quaternion &q);   // 惯性-物体

    // 矩阵→欧拉角 10.6.2
    void fromObjectToWorldMatrix(const Matrix4x3 &m); // 物体-世界坐标系
    void fromWorldToObjectMatrix(const Matrix4x3 &m); // 世界-物体坐标系

    // 旋转矩阵→欧拉角
    void fromRotationMatrix(const RotationMatrix &m);
};

// 单位欧拉角
extern const EulerAngles kEulerAnglesIdentity;

#endif