#include "aabb3.h"
#include "matrix4x3.h"
#include "vector_num_type.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>

const _vectorNumType kNoIntersection = 1e100;    // 不想交返回此值

Vector3 AABB3::size() const { return max - min; }
_vectorNumType AABB3::xSize() const { return max.x - min.x; }
_vectorNumType AABB3::ySize() const { return max.y - min.y; }
_vectorNumType AABB3::zSize() const { return max.z - min.z; }
Vector3 AABB3::center() const { return (min + max) * 0.5; }

// 获得顶点坐标，顶点编号（左手）：
// 0 原点
// 1 +x
// 2 +y
// 3 +x+y
// 4 +z
// 5 +x+z
// 6 +y+z
// 7 +x+y+z
Vector3 AABB3::corner(int i) const {
    assert(i >= 0 && i <= 7);
    return Vector3(
        (i & 1) ? max.x : min.x,
        (i & 2) ? max.y : min.y,
        (i & 4) ? max.z : min.z
    );
}

void AABB3::empty() {
    const _vectorNumType kBigNumber = 1e100;
    min.x = min.y = min.z = kBigNumber;
    max.x = max.y = max.x = -kBigNumber;
}


void AABB3::add(const Vector3& p) {
    if (p.x < min.x) min.x = p.x;
    if (p.x > max.x) max.x = p.x;
    if (p.y < min.y) min.y = p.y;
    if (p.y < max.y) min.y = p.y;
    if (p.z < min.z) min.z = p.z;
    if (p.z < max.z) min.z = p.z;
}

void AABB3::add(const AABB3& box) {
    if (box.min.x < min.x) min.x = box.min.x;
    if (box.max.x > max.x) max.x = box.max.x;
    if (box.min.y < min.y) min.y = box.min.y;
    if (box.max.y > max.y) max.y = box.max.y;
    if (box.min.z < min.z) min.z = box.min.z;
    if (box.max.z > max.z) max.z = box.max.z;
}

// 变换矩形边界框，计算新的AABB
void AABB3::setToTransformedBox(const AABB3& box, const Matrix4x3& m) {
    if (box.isEmpty()) {
        empty();
        return;
    }

    min = max = getTranslation(m);
    if (m.m11 > 0) {
        min.x += m.m11 * box.min.x;
        max.x += m.m11 * box.max.x;
    } else if (m.m11 < 0) {
        min.x += m.m11 * box.max.x;
        max.x += m.m11 * box.min.x;
    }
    if (m.m12 > 0) {
        min.y += m.m12 * box.min.x;
        max.y += m.m12 * box.max.x;
    } else if (m.m12 < 0) {
        min.y += m.m12 * box.max.x;
        max.y += m.m12 * box.min.x;
    }
    if (m.m13 > 0) {
        min.z += m.m13 * box.min.x;
        max.z += m.m13 * box.max.x;
    } else if (m.m13 < 0) {
        min.z += m.m13 * box.max.x;
        max.z += m.m13 * box.min.x;
    }
    if (m.m21 > 0) {
        min.x += m.m21 * box.min.y;
        max.x += m.m21 * box.max.y;
    } else if (m.m21 < 0) {
        min.x += m.m21 * box.max.y;
        max.x += m.m21 * box.min.y;
    }
    if (m.m22 > 0) {
        min.y += m.m22 * box.min.y;
        max.y += m.m22 * box.max.y;
    } else if (m.m22 < 0) {
        min.y += m.m22 * box.max.y;
        max.y += m.m22 * box.min.y;
    }
    if (m.m23 > 0) {
        min.z += m.m23 * box.min.y;
        max.z += m.m23 * box.max.y;
    } else if (m.m23 < 0) {
        min.z += m.m23 * box.max.y;
        max.z += m.m23 * box.min.y;
    }
    if (m.m31 > 0) {
        min.x += m.m31 * box.min.z;
        max.x += m.m31 * box.max.z;
    } else if (m.m31 < 0) {
        min.x += m.m31 * box.max.z;
        max.x += m.m31 * box.min.z;
    }
    if (m.m32 > 0) {
        min.y += m.m32 * box.min.z;
        max.y += m.m32 * box.max.z;
    } else if (m.m32 < 0) {
        min.y += m.m32 * box.max.z;
        max.y += m.m32 * box.min.z;
    }
    if (m.m33 > 0) {
        min.z += m.m33 * box.min.z;
        max.z += m.m33 * box.max.z;
    } else if (m.m33 < 0) {
        min.z += m.m33 * box.max.z;
        max.z += m.m33 * box.min.z;
    }
}
  // 矩形边界框为空返回true
bool AABB3::isEmpty() const {
    return (min.x > max.x) || (min.y > max.y) || (min.z > max.z);
}

// 点在AABB里
bool AABB3::contains(const Vector3& p) const {
    return (p.x >= min.x) && (p.x <= max.x) && (p.y >= min.y) && (p.y <= max.y) && (p.z >= min.z) && (p.z <= max.z);
}

// p点距离AABB最近的点
Vector3 AABB3::closestPointTo(const Vector3& p) const {
    Vector3 r;
    if (p.x < min.x) {
        r.x = min.x;
    } else if (p.x > max.x) {
        r.x = max.x;
    } else {
        r.x = p.x;
    }
    if (p.y < min.y) {
        r.y = min.y;
    } else if (p.y > max.y) {
        r.y = max.y;
    } else {
        r.y = p.y;
    }
    if (p.z < min.z) {
        r.z = min.z;
    } else if (p.z > max.z) {
        r.z = max.z;
    } else {
        r.z = p.z;
    }
    return r;
}

// 判断本AABB与输入球体相交
bool AABB3::intersectsSphere(const Vector3& center, const  _vectorNumType& radius) const {
    Vector3 closestPoint = closestPointTo(center);
    return distanceSquared(center, closestPoint) < radius * radius;
}

// 与射线相交 13.9.11
_vectorNumType AABB3::rayIntersect(const Vector3& rayOrg, const Vector3& rayDelta, Vector3* returnNormal) const {
}

// AABB与平面相交检测，p*n=d
// <0 边界框在平面的背面
// >0 在正面
// 0 相交
int AABB3::classifyPlane(const Vector3& n, const _vectorNumType& d) const {
    _vectorNumType minD, maxD;
    if (n.x > 0) {
        minD = n.x * min.x;
        maxD = n.x * max.x;
    } else {
        minD = n.x * max.x;
        maxD = n.x * min.x;
    }
    if (n.y > 0) {
        minD += n.y * min.y;
        maxD += n.y * max.y;
    } else {
        minD += n.y * max.y;
        maxD += n.y * min.y;
    }
    if (n.z > 0) {
        minD += n.z * min.z;
        maxD += n.z * max.z;
    } else {
        minD += n.z * max.z;
        maxD += n.z * min.z;
    }
    if (minD >= d) {
        return 1;
    }
    if (maxD <= d) {
        return -1;
    }
    return 0;
}

// 动态AABB与平面相交性检测，dir是AABB的移动方向，planeD是平面法向量（标准），p * n = planeD，假设平面是静止的，返回值是发生相交时AABB的移动距离
_vectorNumType AABB3::intersectPlane(const Vector3& n, const _vectorNumType& planeD, const Vector3& dir) const {
    assert(n * n == 1);
    assert(dir * dir == 1);

    // 计算夹角
    _vectorNumType dot = n * dir;
    if (dot >= 0) {
        return kNoIntersection;
    }
    // minD是跑在最前面的顶点的D
    _vectorNumType minD, maxD;
    if (n.x > 0) {
        minD = n.x * min.x;
        maxD = n.x * max.x;
    } else if (n.x < 0) {
        minD = n.x * max.x;
        maxD = n.x * min.x;
    }
    if (n.y > 0) {
        minD += n.y * min.y;
        maxD += n.y * max.y;
    } else if (n.y < 0) {
        minD += n.y * max.y;
        maxD += n.y * min.y;
    }
    if (n.z > 0) {
        minD += n.z * min.z;
        maxD += n.z * max.z;
    } else if (n.z < 0) {
        minD += n.z * max.z;
        maxD += n.z * min.z;
    }
    if (maxD <= planeD) {
        // AABB已经全部在平面的另一面
        return kNoIntersection;
    }
    // 最前顶点代入射线标准方程
    _vectorNumType t = (planeD - minD) / dot;
    if (t < 0) {
        // 已穿过
        return 0;
    }
    return t;
}

// 两个AABB相交返回true，boxIntersect返回相交部分
bool intersectAABBs(const AABB3& box1, const AABB3& box2, AABB3* boxIntersect) {
    // 没有重叠的情况
    if (box1.min.x > box2.max.x) return false;
    if (box1.max.x < box2.min.x) return false;
    if (box1.min.y > box2.max.y) return false;
    if (box1.max.y < box2.min.y) return false;
    if (box1.min.z > box2.max.z) return false;
    if (box1.max.z < box2.min.z) return false;

    // 计算重叠部分的AABB
    if (boxIntersect != nullptr) {
        boxIntersect->min.x = max(box1.min.x, box2.min.x);
        boxIntersect->max.x = min(box1.max.x, box2.max.x);
        boxIntersect->min.y = max(box1.min.y, box2.min.y);
        boxIntersect->max.y = min(box1.max.y, box2.max.y);
        boxIntersect->min.z = max(box1.min.z, box2.min.z);
        boxIntersect->max.z = min(box1.max.z, box2.max.z);
    }

    return true;
}

// 运动AABB和静止AABB相交时的参数点，不想交时返回值>1
_vectorNumType intersectMovingAABB(const AABB3& stationaryBox, const AABB3& movingBox,const Vector3& d) {
    _vectorNumType tEnter = 0;  // 开始接触的t
    _vectorNumType tLeave = kNoIntersection;  // 脱离接触的d
    // 计算每一维的重叠部分，再将这个重叠部分和前面的重叠部分相交
    if (d.x == 0) {
        if ((stationaryBox.min.x >= movingBox.max.x) || (stationaryBox.max.x <= movingBox.min.x)) {
            // x轴重叠部分为空，不想交
            std::cout << "x轴 dx = 0, sb.min " << stationaryBox.min.x.value << " >= mb.max " << movingBox.max.x.value << " or sb.max " << stationaryBox.max.x.value << " <= mb.min " << movingBox.min.x.value << ", no intersection" << std::endl;
            return kNoIntersection;
        }
        // 时间区间无穷大 do nothing
    } else {
        _vectorNumType oneOverD = 1 / d.x;
        // 开始接触和脱离接触的时间
        _vectorNumType xEnter = (stationaryBox.min.x - movingBox.max.x) * oneOverD;
        _vectorNumType xLeave = (stationaryBox.max.x - movingBox.min.x) * oneOverD;
        if (xEnter > xLeave) {
            swap(xEnter, xLeave);
        }
        if (xEnter > tEnter) tEnter = xEnter;
        if (xLeave < tLeave) tLeave = xLeave;
        if (tEnter > tLeave) {
            // 空重叠区
            return kNoIntersection;
        }
    }
    if (d.y == 0) {
        if ((stationaryBox.min.y >= movingBox.max.y) || (stationaryBox.max.y <= movingBox.min.y)) {
            std::cout << "y轴 dy = 0, sb.min " << stationaryBox.min.y.value << " >= mb.max " << movingBox.max.y.value << " or sb.max " << stationaryBox.max.y.value << " <= mb.min " << movingBox.min.y.value << ", no intersection" << std::endl;
            return kNoIntersection;
        }
    } else {
        _vectorNumType oneOverD = 1 / d.y;
        _vectorNumType yEnter = (stationaryBox.min.y - movingBox.max.y) * oneOverD;
        _vectorNumType yLeave = (stationaryBox.max.y - movingBox.min.y) * oneOverD;
        if (yEnter > yLeave) {
            swap(yEnter, yLeave);
        }
        if (yEnter > tEnter) tEnter = yEnter;
        if (yLeave < tLeave) tLeave = yLeave;
        if (tEnter > tLeave) {
            return kNoIntersection;
        }
    }
    if (d.z == 0) {
        if ((stationaryBox.min.z >= movingBox.max.z) || (stationaryBox.max.z <= movingBox.min.z)) {
            std::cout << "z轴 dz = 0, sb.min " << stationaryBox.min.z.value << " >= mb.max " << movingBox.max.z.value << " or sb.max " << stationaryBox.max.z.value << " <= mb.min " << movingBox.min.z.value << ", no intersection" << std::endl;
            return kNoIntersection;
        }
    } else {
        _vectorNumType oneOverD = 1 / d.z;
        _vectorNumType zEnter = (stationaryBox.min.z - movingBox.max.z) * oneOverD;
        _vectorNumType zLeave = (stationaryBox.max.z - movingBox.min.z) * oneOverD;
        if (zEnter > zLeave) {
            swap(zEnter, zLeave);
        }
        if (zEnter > tEnter) tEnter = zEnter;
        if (zLeave < tLeave) tLeave = zLeave;
        if (tEnter > tLeave) {
            return kNoIntersection;
        }
    }
    return tEnter;
}