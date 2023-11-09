
/*****************************************************************************************/
inline float VectorMag(const Vector2D &a)
{
	return sqrtf(a.x*a.x + a.y*a.y);
}

/*****************************************************************************************/
inline Vector2D operator *(float k, const Vector2D &v)
{
	return Vector2D(k*v.x, k*v.y);
}