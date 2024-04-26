#include <iostream>
#include <vector>
#include "aabb3.h"

int main() {
    AABB3 a,b;
    a.min = Vector3(0,0,0);
    a.max = Vector3(1,1,1.67);
    b.min = Vector3(0.5,0.5,10);
    b.max = Vector3(1.5,1.5,11);
    std::cout << intersectMovingAABB(a,b,Vector3(0,0,1)).value << std::endl;
    return 0;
}