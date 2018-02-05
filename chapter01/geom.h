#ifndef _GEOM_H
#define _GEOM_H

#include <cmath>

template <class T> struct Vector2 {
	union {
		struct { T u, v; };
		struct { T x, y; };
		T raw[2];
	};

	Vector2() : u(0), v(0) {}
	Vector2(T _u, T _v) : u(_u), v(_v) {}

	inline Vector2<T> operator +(const Vector2<T> &vec) const { return Vector2<T>( u+vec.u, v+vec.v ); }
	inline Vector2<T> operator -(const Vector2<T> &vec) const { return Vector2<T>( u-vec.u, v-vec.v ); }
	inline Vector2<T> operator *(float f) const { return Vector2<T>( u*f, v*f ); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vector2<T>& v);
};

template <class T> struct Vector3 {
	union {
		struct { T x, y, z; };
		struct { T iver, iui, inorm; };
		T raw[3];
	};

	Vector3() : x(0), y(0), z(0) {}
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	inline Vector3<T> operator +(const Vector3<T> &v) const { return Vector3<T>( x+v.x, y+v.y, z+v.z ); }
	inline Vector3<T> operator -(const Vector3<T> &v) const { return Vector3<T>( x-v.x, y-v.y, z-v.z ); }
	inline Vector3<T> operator *(float f) const { return Vector3<T>( x*f, y*f, z*f ); }

	inline T operator *(const Vector3<T> &vec) const { return x*vec.x + y*vec.y + z*vec.z; }
	inline Vector3<T> operator ^(const Vector3<T> &v) const { return Vector3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }

	float norm() const { return std::sqrt(x*x + y*y + z*z); }
	Vector3<T>& normalize(T l = 1) { *this = (*this) * (l/norm()); return *this; }

	template <class > friend std::ostream& operator<<(std::ostream& s, Vector3<T>& v);
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

template <class > std::ostream& operator<<(std::ostream& s, Vector2<T>& v) {
	s << "(" << v.x << "," << v.y << ")\n";
	return s;
}

template <class > std::ostream& operator<<(std::ostream& s, Vector3<T>& v) {
	s << "(" << v.x << "," << v.y << "," << v.z << ")\n";
	return s;
}

#endif // _GEOM_H