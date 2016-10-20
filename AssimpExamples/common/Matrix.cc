#include <Matrix.h>
#include <cstdint>
#include <memory>

namespace sess
{

const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f, 0.f,  0.f, 0.f, 1.f, 0.f,  0.f, 0.f, 0.f, 1.f };

Matrix::Matrix()
	: _11(0.f), _12(0.f), _13(0.f), _14(0.f)
	, _21(0.f), _22(0.f), _23(0.f), _24(0.f)
	, _31(0.f), _32(0.f), _33(0.f), _34(0.f)
	, _41(0.f), _42(0.f), _43(0.f), _44(0.f)
{}

Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
	: _11(m11), _12(m12), _13(m13), _14(m14)
	, _21(m21), _22(m22), _23(m23), _24(m24)
	, _31(m31), _32(m32), _33(m33), _34(m34)
	, _41(m41), _42(m42), _43(m43), _44(m44)
{}

Matrix Matrix::Transpose() const
{
	Matrix tr;

	for (std::uint8_t r = 0u; r < 4u; r++)
	{
		for (std::uint8_t c = 0u; c < 4u; c++)
		{
			tr.m[r][c] = m[c][r];
		}
	}

	return tr;
}

// Assumes regular 4x4 matrix, with 0 0 0 1 as the bottom row
float Matrix::Determinant() const
{
	return
		_11 * (_22 * _33 - _23 * _32)
		- _12 * (_21 * _33 - _13 * _31)
		+ _13 * (_21 * _32 - _23 * _31);
}

Matrix Matrix::operator*(const Matrix & m2) const
{
	Matrix tr;

	for (std::uint8_t row = 0u; row < 4; row++)
	{
		for (std::uint8_t col = 0u; col < 4; col++)
		{
			tr.m[row][col] = 0.f;
			for (std::uint8_t k = 0u; k < 4; k++)
			{
				tr.m[row][col] += m[row][k] * m2.m[k][col];
			}
		}
	}

	return std::move(tr);
}

};