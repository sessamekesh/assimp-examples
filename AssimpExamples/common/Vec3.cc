// I am using .cc instead of .cpp because that's what the Google style guide says,
//  and of course I always want to try to impress Google.
// I'm even remarkably happy with my current employment at Lucid Software (as of writing this)
//  and don't see myself leaving for at least a few more years. It's not even a job thing.
//  It's just... I want Google to like me, that's all.

// NOTICE ME, SENPAI!

// Anyways.

#include <Vec3.h>

#include <cmath>

namespace sess
{

const Vec3 Vec3::Zero = Vec3(0.f, 0.f, 0.f);
const Vec3 Vec3::UnitX = Vec3(1.f, 0.f, 0.f);
const Vec3 Vec3::UnitY = Vec3(0.f, 1.f, 0.f);
const Vec3 Vec3::UnitZ = Vec3(0.f, 0.f, 1.f);
const Vec3 Vec3::Ones = Vec3(1.f, 1.f, 1.f);

Vec3::Vec3() : x(0.f), y(0.f), z(0.f) {}
Vec3::Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

Vec3 Vec3::operator+(const Vec3& o) const
{
	return Vec3(x + o.x, y + o.y, z + o.z);
}

Vec3 Vec3::operator-(const Vec3 & o) const
{
	return Vec3(x - o.x, y - o.y, z - o.z);
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator*(float s) const
{
	return Vec3(x * s, y * s, z * s);
}

Vec3 & Vec3::operator+=(const Vec3 & o)
{
	x += o.x;
	y += o.y;
	z += o.z;
	return *this;
}

Vec3 & Vec3::operator-=(const Vec3 & o)
{
	x -= o.x;
	y -= o.y;
	z -= o.z;
	return *this;
}

Vec3 & Vec3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

float Vec3::Dot(const Vec3 & _1, const Vec3 & _2)
{
	return
		_1.x * _2.x +
		_1.y * _2.y +
		_1.z * _2.z;
}

Vec3 Vec3::Cross(const Vec3 & _1, const Vec3 & _2)
{
	return Vec3(
		_1.y * _2.z - _1.z *_2.y,
		_1.z * _2.x - _1.x * _2.z,
		_1.x * _2.y - _1.y * _2.x
		);
}

Vec3 Vec3::ComponentProduct(const Vec3 & l, const Vec3 & r)
{
	return Vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

float Vec3::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z);
}

Vec3 Vec3::Normal() const
{
	return *this * (1.f / Magnitude());
}

};

