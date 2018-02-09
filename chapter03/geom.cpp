
#include <cassert>
#include <cmath>
#include <iostream>
#include "geom.h"

template <> template <> Vector3<int>::Vector3(const Vector3<float> &v) : x(int(v.x+.5)), y(int(v.y+.5)), z(int(v.z+.5)) {
}

template <> template <> Vector3<float>::Vector3(const Vector3<int> &v) : x(v.x), y(v.y), z(v.z) {
}