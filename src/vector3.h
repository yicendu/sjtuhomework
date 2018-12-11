#pragma once

#ifndef __min
#define __min(a,b) ((a)<(b)?(a):(b);
#endif

#ifndef __max
#define __max(a,b) ((a)>(b)?(a):(b);
#endif

#include <cmath>
#include <iostream>
using namespace std;

// classes declaration
template <class C> class Vector3;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

// class Vector3 definition
template <class C>
class Vector3 {
public:
	C x;
	C y;
	C z;
public:
	// constructors
	Vector3() { x = y= z = 0; }
	Vector3(const C & value) { x = y = z = value; }
	Vector3(const C & a, const C & b, const C & c) {
		x = a;
		y = b;
		z = c;
	}
	Vector3(C arr[]) {
		x = arr[0];
		y = arr[1];
		z = arr[2];
	}
	Vector3(const Vector3<C> & right) {
		x = right.x;
		y = right.y;
		z = right.z;
	}


	// binary operators
	Vector3<C> operator+ (const Vector3<C> & r) const { return Vector3<C>(x+r.x, y+r.y, z+r.z); }
	Vector3<C> operator- (const Vector3<C> & r) const { return Vector3<C>(x-r.x, y-r.y, z-r.z); }
	Vector3<C> operator* (const C & r) const { return Vector3<C>(x*r, y*r, z*r); }

	friend Vector3<C> operator*(const C & l, const Vector3<C> & r);

	//friend Vector3<C> operator*(double mul, const Vector3<C> & r);

	Vector3<C> operator/ (const C & r) const { return Vector3<C>(x/r, y/r, z/r); }


	// unary operators
	Vector3<C> operator-() const { return Vector3<C>(-x, -y, -z); } 


	// assignment operations
	Vector3<C> & operator= (const Vector3<C> & r) { x=r.x; y=r.y; z=r.z; return *this; }
	Vector3<C> & operator+= (const Vector3<C> & r) { return (*this) = (*this) + r; }
	Vector3<C> & operator-= (const Vector3<C> & r) { return (*this) = (*this) - r; }
	Vector3<C> & operator*= (const C & r) { return (*this) = (*this) * r; }
	Vector3<C> & operator/= (const C & r) { return (*this) = (*this) / r; }
	bool operator==(const Vector3<C>& other) const {
		return other.x == x && other.y == y && other.z == z;
	}

	C operator[](const int& index) const {
		switch (index)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return 0;
			break;
		}
	}
	// dot and cross products
	C Dot(const Vector3<C> & r) const { return x*r.x + y*r.y + z*r.z; }
	Vector3<C> Cross(const Vector3<C> & r) const { 
		return Vector3<C>(	y * r.z - z * r.y,
							z * r.x - x * r.z,
							x * r.y - y * r.x ); 
	} 


	// norm (lengths) functions
	C L1Norm() const { return fabs(x) + fabs(y) + fabs(z);}
	C L2Norm() const { return sqrt(Dot(*this));}
	C Distance(const Vector3<C> & r) const { return (*this-r).L2Norm();} 

	//judge zero vector
	bool Iszero() {
		if (x == 0 && y == 0 && z == 0) { return true; }
		else { return false; }
	}


	// friends operators
	friend Vector3<C> operator*(const C & l, const Vector3<C> & r) {
		return Vector3<C>(l*r.x, l*r.y, l*r.z);
	}

	friend ostream & operator<< (ostream & out, const Vector3<C> & r) {
		return out << r.x << " " << r.y << " " << r.z;
	}
};

template <class C>
Vector3<C> min(const Vector3<C> a, const Vector3<C> b, const Vector3<C> c) {
	return Vector3<C>(__min(__min(a.x, b.x), c.x), __min(__min(a.y, b.y), c.y), __min(__min(a.z, b.z), c.z));
}

template <class C>
Vector3<C> max(const Vector3<C> a, const Vector3<C> b, const Vector3<C> c) {
	return Vector3<C>(__max(__max(a.x, b.x), c.x), __max(__max(a.y, b.y), c.y), __max(__max(a.z, b.z), c.z));
}

template <class C>
float max(const Vector3<C> a) {
	return (__max(__max(a.x, a.y), a.z));
}

#undef __max
#undef __min