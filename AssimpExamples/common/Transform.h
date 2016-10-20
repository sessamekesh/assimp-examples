#pragma once

// Represents a 3D point transform - translation, rotation, scaling.
// It's much easier to work with this kind of class on the logical side, and
//  convert to a matrix before sending to the GPU.
// Realistically matrices could be exclusively used, but it's much easier to debug
//  and reason about code in this format.
// Is it faster or slower? No clue. I actually haven't profiled it.

#include <MathExtras.h>

namespace sess
{

struct Transform
{
public:
	Vec3 Position;
	Quaternion Rotation;
	Vec3 Scale;

public:
	Transform();
	Transform(const Vec3& pos, const Quaternion& rot, const Vec3& scale);
	Transform(const Transform&) = default;
	~Transform() = default;

	// Combine two transformations together
	Transform operator*(const Transform& rhs) const;

	Matrix GetTransformMatrix() const;
	Transform Inverse() const;
	static Transform FromTransformMatrix(const Matrix& m);

	// Interpolate between the two transformations. A ratio of 0 means use t1,
	//  a ratio of 1 means t2, and something in the middle means mix the two.
	// 0.25 means use 25% of t1, and 75% of t2, for example.
	// Position and scale interpolated linearly, rotation interpolated spherically
	static Transform Lerp(const Transform& t1, const Transform& t2, float ratio);

public:
	const static Transform Identity;
};

};