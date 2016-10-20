#pragma once

// 3D vector class, does all the 3D vector things
// May represent either a point or a direction
// May be transformed by various other components

namespace sess
{

struct Vec3
{
public:
	float x, y, z;

public:
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(const Vec3&) = default;
	~Vec3() = default;

	Vec3 operator+(const Vec3&) const;
	Vec3 operator-(const Vec3&) const;
	Vec3 operator-() const;
	Vec3 operator*(float) const;

	Vec3& operator+=(const Vec3&);
	Vec3& operator-=(const Vec3&);
	Vec3& operator*=(float);

	static float Dot(const Vec3&, const Vec3&);
	static Vec3 Cross(const Vec3&, const Vec3&);
	static Vec3 ComponentProduct(const Vec3&, const Vec3&);

	float Magnitude() const;
	Vec3 Normal() const;

public:
	const static Vec3 Zero;
	const static Vec3 Ones;
	const static Vec3 UnitX;
	const static Vec3 UnitY;
	const static Vec3 UnitZ;
};

};