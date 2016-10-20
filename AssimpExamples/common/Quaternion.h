#pragma once

// Quaternions! These are a special form of 4D vector that are used to store
//  information about rotations. They're pretty fast, elegant, and have a much
//  higher tolerance to bullshit than matrices do.
// https://en.wikipedia.org/wiki/Quaternion

#include <Vec3.h>
#include <Matrix.h>

namespace sess
{

struct Quaternion
{
public:
	float x, y, z, w;

public:
	Quaternion();
	Quaternion(Vec3 axis, float angle);
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Quaternion&) = default;
	~Quaternion() = default;

	Quaternion Inverse() const;

	Quaternion operator*(const Quaternion&) const;
	Quaternion& operator*=(const Quaternion&);

	static Quaternion FromMatrix(const Matrix& m);

	static const Quaternion Identity;

protected:
	void Normalize();
};

};