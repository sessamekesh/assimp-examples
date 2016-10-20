#include <FreeCamera.h>
#include <Quaternion.h>
#include <MathExtras.h>

namespace sess
{

FreeCamera::FreeCamera(const Vec3& position, const Vec3& lookAt, const Vec3& up)
	: position_(position)
	, forward_((position + lookAt).Normal())
	, up_(up.Normal())
	, right_(Vec3::Cross(position + lookAt, up).Normal())
	, dViewMatrix_({ Matrix::Identity, true })
{}

Matrix FreeCamera::GetViewMatrix()
{
	if (dViewMatrix_.isDirty)
	{
		dViewMatrix_.viewMatrix = LookAtLH(position_, position_ + forward_, up_);
		dViewMatrix_.isDirty = false;
	}

	return dViewMatrix_.viewMatrix;
}

Vec3 FreeCamera::GetPosition() const
{
	return position_;
}

void FreeCamera::MoveForward(float distance)
{
	// Here is one of the reasons I love C++ operator overloading.
	// This here makes really intuitive sense to a beginner looking at it
	// At least, it did for me. Move the position in the direction of forward by some distance "distance"
	position_ += forward_ * distance;
}

void FreeCamera::MoveRight(float distance)
{
	position_ += right_ * distance;
}

void FreeCamera::MoveUp(float distance)
{
	position_ += up_ * distance;
}

void FreeCamera::RotateRight(float angle)
{
	// The up axis doesn't change, so just rotate about that
	Quaternion rotation(up_, angle);
	forward_ = (forward_ * rotation).Normal();

	right_ = Vec3::Cross(up_, forward_).Normal();

	dViewMatrix_.isDirty = true;
}

void FreeCamera::RotateUp(float angle)
{
	// The right axis should be parallel to the ground, so it can safely be used.
	Quaternion rotation(right_, angle);
	Vec3 newForward = (forward_ * rotation).Normal();

	if (Vec3::Cross(newForward, up_).Magnitude() < 0.01f)
	{
		// The two vectors are too close for comfort. Don't apply this operation
		return;
	}
	forward_ = newForward;
	right_ = Vec3::Cross(up_, forward_).Normal();

	dViewMatrix_.isDirty = true;
}

};