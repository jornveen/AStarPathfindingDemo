#include "Matrix44.h"
#include <iostream>

igad::Matrix44::Matrix44(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	this->xAxis = Vector3(m00, m01, m02);
	this->wx = m03;
	
	this->yAxis = Vector3(m10, m11, m12);
	this->wy = m13;

	this->zAxis = Vector3(m20, m21, m22);
	this->wz = m23;

	this->translation = Vector3(m30, m31, m32);
	this->one = m33;

}

igad::Matrix44::Matrix44()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

igad::Vector3 igad::Matrix44::operator*(const Vector3 & vec) const
{
	return Vector3(	xAxis.x * vec.x + xAxis.y * vec.y + xAxis.z * vec.z,
					yAxis.x * vec.x + yAxis.y * vec.y + yAxis.z * vec.z,
					zAxis.x * vec.x + zAxis.y * vec.y + zAxis.z * vec.z);
}

igad::Matrix44 igad::Matrix44::operator+(const Matrix44 & mat) const
{
	Matrix44 result;

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			result.m[i][j] = m[i][j] + mat.m[i][j];
		}
	}

	return result;
}

igad::Matrix44 igad::Matrix44::operator-(const Matrix44 & mat) const
{
	Matrix44 result;

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			result.m[i][j] = m[i][j] - mat.m[i][j];
		}
	}

	return result;
}

igad::Matrix44 igad::Matrix44::operator*(const Matrix44 & mat) const
{
	Matrix44 result = Matrix44(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	for (int column = 0; column < 4; column++) 
	{
		for (int row = 0; row < 4; row++) 
		{
			for (int inner = 0; inner < 4; inner++) 
			{
				result.m[row][column] += mat.m[row][inner] * m[inner][column];
			}
		}
	}

	return result;
}

igad::Vector3 igad::Matrix44::GetTranslation() const
{
	return this->translation;
}

void igad::Matrix44::SetTranslation(const Vector3 & vec)
{
	this->translation = vec;
}

igad::Vector3 igad::Matrix44::GetXAxis() const
{
	return this->xAxis;
}

igad::Vector3 igad::Matrix44::GetYAxis() const
{
	return this->yAxis;
}

igad::Vector3 igad::Matrix44::GetZAxis() const
{
	return this->zAxis;
}

float igad::Matrix44::Determinant() const
{
	float a = m[0][0], b = m[0][1], c = m[0][2], d = m[0][3];
	float e = m[1][0], f = m[1][1], g = m[1][2], h = m[1][3];
	float i = m[2][0], j = m[2][1], k = m[2][2], l = m[2][3];
	float mm = m[3][0], n = m[3][1], o = m[3][2], p = m[3][3];


	float result =
		(a * (f * (k*p - o*l) - g * (j*p - n*l) + h * (j*o - k*n)))
		- (b * (e * (k*p - o*l) - g * (i*p - mm*l) + h * (i*o - mm*k)))
		+ (c * (e * (j*p - l*n) - f * (i*p - mm*l) + h * (i*n - mm*j)))
		- (d * (e * (j*o - n*k) - f * (i*o - mm*k) + g * (i*n - mm*j)));

	return result;
}

bool igad::Matrix44::Invert()
{
	float dis = Determinant();
	if (dis == 0) {
		return false;
	}
	else {
		Matrix44 result;
		float a = m[0][0], b = m[0][1], c = m[0][2], d = m[0][3];
		float e = m[1][0], f = m[1][1], g = m[1][2], h = m[1][3];
		float i = m[2][0], j = m[2][1], k = m[2][2], l = m[2][3];
		float mm = m[3][0], n = m[3][1], o = m[3][2], p = m[3][3];

		result.m[0][0] = (f * (k*p - o*l) - g * (j*p - n*l) + h * (j*o - n*k));
		result.m[0][1] = (e * (k*p - o*l) - g * (i*p - mm*l) + h * (i*o - mm*k));
		result.m[0][2] = (e * (j*p - n*l) - f * (i*p - mm*l) + h * (i*n - mm*j));
		result.m[0][3] = (e * (j*o - n*k) - f * (i*o - mm*k) + g * (i*n - mm*j));

		result.m[1][0] = (b * (k*p - o*l) - c * (j*p - n*l) + d * (j*o - n*k));
		result.m[1][1] = (a * (k*p - o*l) - c * (i*p - mm*l) + d * (i*o - mm*k));
		result.m[1][2] = (a * (j*p - n*l) - b * (i*p - mm*l) + d  * (i*n - mm*j));
		result.m[1][3] = (a * (j*o - n*k) - b * (i*o - mm*k) + c * (i*n - mm*j));

		result.m[2][0] = (b * (g*p - o*h) - c * (f*p - n*h) + d * (f*o - n*g));
		result.m[2][1] = (a * (g*p - o*h) - c * (e*p - mm*h) + d * (e*o - mm*g));
		result.m[2][2] = (a * (f*p - n*h) - b * (e*p - mm*h) + d * (e*n - mm*f));
		result.m[2][3] = (a * (f*o - n*g) - b * (e*o - mm*g) + c * (e*n - mm*f));

		result.m[3][0] = (b * (g*l - k*h) - c * (f*l - j*h) + d * (f*k - j*g));
		result.m[3][1] = (a * (g*l - k*h) - c * (e*l - i*h) + d * (e*k - i*g));
		result.m[3][2] = (a * (f*l - j*h) - b * (e*l - i*h) + d * (e*j - f*i));
		result.m[3][3] = (a * (f*k - j*g) - b * (e*k - i*g) + c * (e*j - f*i));

		for (int row = 0; row < 4; row++) 
		{
			for (int column = 0; column < 4; column++) 
			{
				if ((row + column) % 2 == 1) 
				{
					result.m[row][column] = -result.m[row][column];
				}
			}
		}
		result.Transpose();

		for (int row = 0; row < 4; row++) 
		{
			for (int column = 0; column < 4; column++)
			{
				result.m[row][column] = result.m[row][column] / dis;
				m[row][column] = result.m[row][column];
			}
		}

		return true;
	}
}

void igad::Matrix44::Transpose()
{
	Matrix44 temp;

	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			temp.m[row][column] = m[column][row];
		}
	}

	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			m[row][column] = temp.m[row][column];
		}
	}
}

void igad::Matrix44::SetOrientation(const Vector3 & x, const Vector3 & y, const Vector3 & z)
{
	xAxis = x;
	yAxis = y;
	zAxis = z;

}

void igad::Matrix44::SetEulerAxis(float yaw, float pitch, float roll)
{
	Matrix44 result = CreateIdentity();

	Matrix44 xAxis = CreateRotateX(pitch);
	Matrix44 yAxis = CreateRotateY(yaw);
	Matrix44 zAxis = CreateRotateZ(roll);

	
	result = (zAxis * yAxis) * xAxis;

	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			m[row][column] = result.m[row][column];
		}
	}
}

igad::Matrix44 igad::Matrix44::CreateIdentity()
{
	Matrix44 result = Matrix44(	1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);

	return result;
}

igad::Matrix44 igad::Matrix44::CreateTranslation(float x, float y, float z)
{
	Matrix44 result = CreateIdentity();
	result.translation = Vector3(x, y, z);

	return result;
}

igad::Matrix44 igad::Matrix44::CreateScale(Vector3 scale)
{
	Matrix44 result;
	result.xAxis.x = scale.x;
	result.yAxis.y = scale.y;
	result.zAxis.z = scale.z;

	return result;
}

igad::Matrix44 igad::Matrix44::CreateRotate(float angle, const Vector3 & axis)
{
	float x2 = axis.x * axis.x;
	float y2 = axis.y * axis.y;
	float z2 = axis.z * axis.z;
	float cosA = cos(angle);
	float sinA = sin(angle);
	float xy = axis.x * axis.y;
	float xz = axis.x * axis.z;
	float yz = axis.y * axis.z;

	Matrix44 result = CreateIdentity();

	result.xAxis.x = x2 - (x2 - 1) * cosA;
	result.xAxis.y = xy - xy * cosA + axis.z * sinA;
	result.xAxis.z = xz - xz * cosA - axis.y * sinA;

	result.yAxis.x = xy - xy * cosA - axis.z * sinA;
	result.yAxis.y = y2 - (y2 - 1) * cosA;
	result.yAxis.z = yz - yz * cosA + axis.x * sinA;

	result.zAxis.x = xz - xz * cosA + axis.y * sinA;
	result.zAxis.y = yz - yz * cosA - axis.x * sinA;
	result.zAxis.z = z2 - (z2 - 1) * cosA;


	return result;
}

igad::Matrix44 igad::Matrix44::CreateRotateX(float angle)
{
	Matrix44 result = CreateIdentity();
	result.yAxis.y = cos(angle);
	result.yAxis.z = sin(angle);
	result.zAxis.y = -sin(angle);
	result.zAxis.z = cos(angle);

	return result;
}

igad::Matrix44 igad::Matrix44::CreateRotateY(float angle)
{
	Matrix44 result = CreateIdentity();
	result.xAxis.x = cos(angle);
	result.xAxis.z = -sin(angle);
	result.zAxis.x = sin(angle);
	result.zAxis.z = cos(angle);

	return result;
}

igad::Matrix44 igad::Matrix44::CreateRotateZ(float angle)
{
	Matrix44 result = CreateIdentity();
	result.xAxis.x = cos(angle);
	result.xAxis.y = sin(angle);
	result.yAxis.x = -sin(angle);
	result.yAxis.y = cos(angle);

	return result;
}

igad::Matrix44 igad::Matrix44::CreateOrtho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	Matrix44 result = CreateIdentity();

	result.xAxis.x = 2.0f / (right - left);
	result.yAxis.y = 2.0f / (top - bottom);
	result.zAxis.z = -2.0f / (farZ - nearZ);

	result.translation.x = -((right + left) / (right - left));
	result.translation.y = -((top +  bottom) / (top - bottom));
	result.translation.z = -((farZ + nearZ) / (farZ - nearZ));

	return result;
}

igad::Matrix44 igad::Matrix44::CreateFrustum(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	Matrix44 result = Matrix44(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

	result.xAxis.x = (2 * nearZ) / (right - left);
	result.xAxis.z = (right + left) / (right - left);
	result.yAxis.y = (2 * nearZ) / (top - bottom);
	result.yAxis.z = (top + bottom) / (top - bottom);
	result.zAxis.z = -(farZ + nearZ) / (farZ - nearZ);

	result.translation.z = (-2 * farZ * nearZ) / (farZ - nearZ);
	result.wz = -1;

	return result;
}

igad::Matrix44 igad::Matrix44::CreatePerspective(float fovy, float aspect, float nearZ, float farZ)
{
	Matrix44 result = Matrix44(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	float f = 1.0f / tan(fovy / 2.0f);

	result.xAxis.x = f / aspect;
	result.yAxis.y = f;
	result.zAxis.z = (farZ + nearZ) / (nearZ - farZ);
	result.translation.z = (2.0f * farZ * nearZ) / (nearZ - farZ);
	result.wz = -1;

	return result;
}

igad::Matrix44 igad::Matrix44::CreateLookAt(const Vector3 & eye, const Vector3 & center, const Vector3 & up)
{
	Vector3 forward = eye - center;
	forward.Normalize();
	Vector3 right = up.Cross(forward);
	right.Normalize();
	
	Matrix44 result(right.x, up.x, forward.x, 0.0f,
					right.y, up.y, forward.y, 0.0f,
					right.z, up.z, forward.z, 0.0f,
					(-right).Dot(eye), (-up).Dot(eye), (-forward).Dot(eye), 1.0f);

	return result;
}

igad::Vector3 igad::Matrix44::TransformDirectionVector(const Vector3 & direction)
{

	return Vector3();
}
