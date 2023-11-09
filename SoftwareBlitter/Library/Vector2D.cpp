#include "Vector2D.h"

/*****************************************************************************************/
Vector2D Vector2D::Zero() 
{ 
	return Vector2D(0.0f, 0.0f); 
}

/*****************************************************************************************/
Vector2D::Vector2D(void)
{
	x = 0;
	y = 0;
}

/*****************************************************************************************/
Vector2D::Vector2D(float xValue, float yValue)
{
	x = xValue;
	y = yValue;
}

/*****************************************************************************************/
Vector2D::Vector2D(const Vector2D& v)
{
	x = v.x;
	y = v.y;
}

/*****************************************************************************************/
Vector2D::~Vector2D(void)
{
	x = 0;
	y = 0;
}
/*****************************************************************************************/
Vector2D& Vector2D::operator =(const Vector2D &v)
{
	x = v.x;
	y = v.y;
	return *this;
}

/*****************************************************************************************/
Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

/*****************************************************************************************/
Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

/*****************************************************************************************/
Vector2D& Vector2D::operator*=(float c)
{
	x *= c;
	y *= c;
	return *this;
}

/*****************************************************************************************/
Vector2D& Vector2D::operator*=(const Vector2D& v)
{
	x *= v.x;	
	y *= v.y;
	return *this;
}

/*****************************************************************************************/
Vector2D Vector2D::operator-() const 
{ 
	return Vector2D(-x, -y); 
}

/*****************************************************************************************/
Vector2D Vector2D::operator+(const Vector2D &a) const 
{
	return Vector2D(x + a.x, y + a.y);
}

/*****************************************************************************************/
Vector2D Vector2D::operator-(const Vector2D &a) const 
{
	return Vector2D(x - a.x, y - a.y);
}

/*****************************************************************************************/
Vector2D Vector2D::operator*(float a) const 
{
	return Vector2D(x*a, y*a);
}

/*****************************************************************************************/
Vector2D Vector2D::operator/(float a) const 
{
	float	oneOverA = 1.0f / a;
	return Vector2D(x*oneOverA, y*oneOverA);
}

/*****************************************************************************************/
float Vector2D::operator*(const Vector2D &a) const 
{
	return x*a.x + y*a.y;
}

/*****************************************************************************************/
void Vector2D::Set(float newX, float newY)
{
	x = newX;
	y = newY;
}

/*****************************************************************************************/
void Vector2D::Normalize()
{
	float magSq = x*x + y*y;

	if (magSq > 0.0f)
	{
		float oneOverMag = 1.0f / sqrtf(magSq);
		x *= oneOverMag;
		y *= oneOverMag;
	}
}
