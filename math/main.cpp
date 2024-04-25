#include<iostream>
#include "vector3.h"
int main() {
    Vector3 v(1,2,3);
    Vector3 v2(1,2,4);
    auto vv = v * v2;
    std::cout << vv.value << std::endl;
    return 0;
}