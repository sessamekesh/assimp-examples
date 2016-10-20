#pragma once

// Camera that is intended to be freely controlled by the player.
//  Methods exposed to move in a certain direction, move forward, to the right, up,
//  rotate about the up axis, or about the axis orthogonal to both the up and forward
//  axis. If up is the forward axis, picks the y-axis... ish

#include <Vec3.h>
#include <Matrix.h>

namespace sess
{

class FreeCamera
{
public:
	FreeCamera(const Vec3& position, const Vec3& lookAt, const Vec3& up);
	// One "prevent yourself from being an idiot" thing that I do is to prohibit copying
	//  objects that I imagine won't need to be copied. This prevents me from accidentally
	//  passing a large object around by value instead of by reference.
	FreeCamera(const FreeCamera&) = delete;
	~FreeCamera() = default;

	Matrix GetViewMatrix(); // May invalidate a cache, so cannot be const
	Vec3 GetPosition() const;

	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);

	void RotateRight(float angle);
	void RotateUp(float angle);

private:
	Vec3 position_;
	Vec3 forward_;
	Vec3 up_;
	Vec3 right_;

	// I like the dirtyable pattern here. This struct stores information
	//  both about the view matrix and a flag indicating if the view matrix
	//  is up to date or not - isDirty will be true if the view matrix needs re-generation
	// Anonymous structs are fine for that :-)
	struct
	{
		Matrix viewMatrix;
		bool isDirty;
	} dViewMatrix_;
};

};