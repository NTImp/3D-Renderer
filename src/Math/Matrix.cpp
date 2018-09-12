#include "Matrix.hpp"

#include "Math.hpp"

namespace Math {
	Vector3::Vector3(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z)
	{ }

	Vector4::Vector4(float _x, float _y, float _z, float _w) :
		x(_x), y(_y), z(_z), w(_w)
	{ }
	
	Vector4::Vector4(Vector3& v, float _w) :
		x(v.x), y(v.y), z(v.z), w(_w)
	{ }

	Matrix4::Matrix4(float d)
	{
		m[0][0] = d;
		m[0][1] = 0;
		m[0][2] = 0;
		m[0][3] = 0;
		
		m[1][0] = 0;
		m[1][1] = d;
		m[1][2] = 0;
		m[1][3] = 0;
		
		m[2][0] = 0;
		m[2][1] = 0;
		m[2][2] = d;
		m[2][3] = 0;
		
		m[3][0] = 0;
		m[3][1] = 0;
		m[3][2] = 0;
		m[3][3] = d;
		
	}

	Matrix4 mul(Matrix4& a, Matrix4& b)
	{
		Matrix4 n;
		for (int i = 0; i < 4; i++)
		{
			for (int e = 0; e < 4; e++)
			{
				n.m[i][e] =  a.m[i][0] * b.m[0][e]
						+ a.m[i][1] * b.m[1][e]
						+ a.m[i][2] * b.m[2][e]
						+ a.m[i][3] * b.m[3][e];
			}
		}
		return n;
	}

	Vector4 mul(Matrix4& a, Vector4& b)
	{
		Vector4 n;
		n.x = a.m[0][0] * b.x + a.m[0][1] * b.y + a.m[0][2] * b.z + a.m[0][3] * b.w;
		n.y = a.m[1][0] * b.x + a.m[1][1] * b.y + a.m[1][2] * b.z + a.m[1][3] * b.w;
		n.z = a.m[2][0] * b.x + a.m[2][1] * b.y + a.m[2][2] * b.z + a.m[2][3] * b.w;
		n.w = a.m[3][0] * b.x + a.m[3][1] * b.y + a.m[3][2] * b.z + a.m[3][3] * b.w;

		return n;
	}

	Matrix4 RotateX(float degrees)
	{
		Matrix4 n(1);
		float c = Cos(degrees);
		float s = Sin(degrees);

		n.m[1][1] = c;
		n.m[1][2] = -s;
		n.m[2][1] = s;
		n.m[2][2] = c;
		return n;
	}

	Matrix4 RotateY(float degrees)
	{
		Matrix4 n(1);
		float c = Cos(degrees);
		float s = Sin(degrees);

		n.m[0][0] = c;
		n.m[0][2] = s;
		n.m[2][0] = s;
		n.m[2][2] = -c;
		return n;
	}

	Matrix4 RotateZ(float degrees)
	{
		Matrix4 n(1);
		float c = Cos(degrees);
		float s = Sin(degrees);

		n.m[0][0] = c;
		n.m[0][1] = s;
		n.m[1][0] = -s;
		n.m[1][1] = c;
		return n;
	}

	Matrix4 Translate(Vector3& translation)
	{
		return Matrix4(1);
	}
};
