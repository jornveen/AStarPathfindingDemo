#include "Vector3.h"


igad::Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

igad::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

igad::Vector3 igad::Vector3::operator+(const Vector3 & v) const
{
	Vector3 temp;
	temp.x = this->x + v.x;
	temp.y = this->y + v.y;
	temp.z = this->z + v.z;

	return temp;
}

igad::Vector3 igad::Vector3::operator-(const Vector3 & v) const
{
	Vector3 temp;
	temp.x = this->x - v.x;
	temp.y = this->y - v.y;
	temp.z = this->z - v.z;

	return temp;
}

igad::Vector3 igad::Vector3::operator*(const float value) const
{
	Vector3 temp;
	temp.x = this->x * value;
	temp.y = this->y * value;
	temp.z = this->z * value;

	return temp;
}

igad::Vector3 igad::Vector3::operator/(const float value) const
{
	Vector3 temp;
	temp.x = this->x / value;
	temp.y = this->y / value;
	temp.z = this->z / value;

	return temp;
}

igad::Vector3 igad::Vector3::operator-() const
{
	Vector3 temp;
	temp.x = -this->x;
	temp.y = -this->y;
	temp.z = -this->z;

	return temp;
}

float igad::Vector3::operator*(const Vector3 & vector) const
{
	return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

void igad::Vector3::operator+=(const Vector3 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void igad::Vector3::operator-=(const Vector3 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void igad::Vector3::operator*=(const float value)
{
	this->x *= value;
	this->y *= value;
	this->z *= value;
}

igad::Vector3 igad::Vector3::Cross(const Vector3 & vector) const
{
	Vector3 cross;
	cross.x = this->y * vector.z - this->z * vector.y;
	cross.y = this->z * vector.x - this->x * vector.z;
	cross.z = this->x * vector.y - this->y * vector.x;

	return cross;
}

float igad::Vector3::Dot(const Vector3 & vector) const
{
	return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

float igad::Vector3::Magnitude() const
{
	float mag = this->x * this->x + this->y * this->y + this->z * this->z;

	return sqrt(mag);
}

float igad::Vector3::SquareMagnitude() const
{
	float mag = Magnitude();
	return mag * mag;
}

void igad::Vector3::Normalize()
{
	float mag = Magnitude();

	if (mag != 0)
	{
		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
	}
}

igad::Vector3 igad::Vector3::GetNormalize()
{
	float mag = Magnitude();
	igad::Vector3 result = Vector3(this->x,this->y,this->z);

	if (mag != 0)
	{
		result.x /= mag;
		result.y /= mag;
		result.z /= mag;
	}
	return result;
}

bool igad::Vector3::operator==(const Vector3 & other) const
{
	return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool igad::Vector3::operator!=(const Vector3 & other) const
{
	return this->x == other.x && this->y == other.y && this->z == other.z;
}

void igad::Vector3::Clear()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

igad::Vector3 igad::Vector3::Slerp(float fact, const Vector3 & r) const
{
	float dot = Dot(r);
	Vector3 start;
	start.x = this->x;
	start.y = this->y;
	start.z = this->z;

	if (dot > 1)
	{
		dot = 1;
	}
	else if (dot < -1)
	{
		dot = -1;
	}

	float theta = acos(dot) * fact;
	Vector3 relativeVec = r - start * dot;
	relativeVec.Normalize();


	return (start * cos(theta)) + (relativeVec * sin(theta));
}

///Vector 2

igad::Vector2::Vector2()
{
	this->x = 0;
	this->y = 0;
}

igad::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

igad::Vector2 igad::Vector2::operator+(const Vector2 & v) const
{
	Vector2 temp;
	temp.x = this->x + v.x;
	temp.y = this->y + v.y;

	return temp;
}

igad::Vector2 igad::Vector2::operator-(const Vector2 & v) const
{
	Vector2 temp;
	temp.x = this->x - v.x;
	temp.y = this->y - v.y;

	return temp;
}

igad::Vector2 igad::Vector2::operator*(const float value) const
{
	Vector2 temp;
	temp.x = this->x * value;
	temp.y = this->y * value;

	return temp;
}

igad::Vector2 igad::Vector2::operator/(const float value) const
{
	Vector2 temp;
	temp.x = this->x / value;
	temp.y = this->y / value;

	return temp;
}

igad::Vector2 igad::Vector2::operator-() const
{
	Vector2 temp;
	temp.x = -this->x;
	temp.y = -this->y;

	return temp;
}

float igad::Vector2::operator*(const Vector2 & vector) const
{
	return this->x * vector.x + this->y * vector.y;
}

void igad::Vector2::operator+=(const Vector2 & v)
{
	this->x += v.x;
	this->y += v.y;
}

void igad::Vector2::operator-=(const Vector2 & v)
{
	this->x -= v.x;
	this->y -= v.y;
}

void igad::Vector2::operator*=(const float value)
{
	this->x *= value;
	this->y *= value;
}

float igad::Vector2::Magnitude() const
{
	float mag = this->x * this->x + this->y * this->y;

	return sqrt(mag);
}

float igad::Vector2::SquareMagnitude() const
{
	float mag = Magnitude();
	return mag * mag;
}

void igad::Vector2::Normalize()
{
	float mag = Magnitude();

	if (mag != 0)
	{
		this->x /= mag;
		this->y /= mag;
	}
}

bool igad::Vector2::operator==(const Vector2 & other) const
{
	return this->x == other.x && this->y == other.y;
}

bool igad::Vector2::operator!=(const Vector2 & other) const
{
	return this->x == other.x && this->y == other.y;
}

void igad::Vector2::Clear()
{
	this->x = 0;
	this->y = 0;
}