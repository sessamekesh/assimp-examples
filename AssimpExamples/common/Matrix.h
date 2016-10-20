#pragma once

// 4x4 matrix class
// DirectX and OpenGL both use lots and lots of matrix mathematics,
//  really it's just sorta an essential part of computer graphics.
// This handles all the mathy stuff.

namespace sess
{

class Matrix
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		float m[4][4];
	};

public:
	Matrix();
	Matrix(const Matrix&) = default;
	Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
	~Matrix() = default;

	Matrix Transpose() const;
	float Determinant() const;

	Matrix operator*(const Matrix& m2) const;

public:
	static const Matrix Identity;
};

};