#include <cmath>

#include "vector_num_type.h"
#include "math_util.h"
#include "vector3.h"

const Vector3 kZeroVector(0,0,0);

_vectorNumType wrapPi(_vectorNumType theta) {
    theta += pi;
    _vectorNumType tmp = theta / (2*pi);
    theta -= floor(tmp.value) * (2*pi);
    theta -= pi;
    return theta;
}

_vectorNumType safeACos(const _vectorNumType& x) {
    if (x <= -1.0) {
        return pi;
    }
    if (x >= 1.0) {
        return 0;
    }
    return acos(x.value);
}