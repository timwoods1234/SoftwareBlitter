#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include "Math.h"

class Vector2D
{
public:
	static Vector2D Zero();

	Vector2D(void);

	Vector2D(float xValue, float yValue);

	Vector2D(const Vector2D& v);

	~Vector2D(void);

	Vector2D& operator=(const Vector2D& v);

	Vector2D& operator+=(const Vector2D& v);

	Vector2D& operator-=(const Vector2D& v);

	Vector2D& operator*=(float c);

	Vector2D& operator*=(const Vector2D& v);

	Vector2D operator-() const;

	Vector2D operator+(const Vector2D &a) const;

	Vector2D operator-(const Vector2D &a) const;

	Vector2D operator*(float a) const;

	Vector2D operator/(float a) const;

	float operator*(const Vector2D &a) const;

	void Normalize();

	void Set(float newX, float newY);

	float x;
	float y;
	
};

inline float VectorMag(const Vector2D &a);

inline Vector2D operator *(float k, const Vector2D &v);

#include "Vector2D.inl"

#endif //__VECTOR2D_H__
