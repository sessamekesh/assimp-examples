#pragma once

// I'm not a fan of putting things here, but the relationships between
//  certain objects isn't exactly tree-structured. There are cycles.
// Ideally, I would just have all my math code in one place.
// Maybe I should have done that instead.
// That's not what these tutorials are about, though.
// I'm already writing hundreds and hundreds of lines of code that I figure
//  won't really be used by the majority of my viewers (thank you to the few of you
//  that are taking the time to look through it <3 love you!)

#include <Matrix.h>
#include <Vec3.h>
#include <Quaternion.h>
#include <Color.h>

namespace sess
{

const float PI = 3.141592653f;

// Operators
Vec3 operator*(const Vec3&, const Quaternion&);

// Helper methods
Matrix PerspectiveLH(float fovY, float aspect, float nearZ, float farZ);
Matrix LookAtLH(const Vec3& pos, const Vec3& lookAt, const Vec3& up);
float Radians(float angle);
float Degrees(float radians);

};