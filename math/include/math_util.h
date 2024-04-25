#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_
#include "vector_num_type.h"
const _vectorNumType pi = 3.14159265358979323846;
// 限制一些角度在-π到π之间
extern _vectorNumType wrapPi(_vectorNumType theta);
extern _vectorNumType safeACos(const _vectorNumType& x);
#endif