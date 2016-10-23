#include <MathExtras.h>

#include <cmath>

namespace sess
{

// http://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
Vec3 operator*(const Vec3 &v, const Quaternion &q)
{
	Vec3 u(q.x, q.y, q.z);

	float s = q.w;

	return u * 2.f * Vec3::Dot(u, v)
		+ v * (s * s - Vec3::Dot(u, u))
		+ Vec3::Cross(u, v) * 2.f * s;
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205350(v=vs.85).aspx
Matrix PerspectiveLH(float fovY, float aspect, float nearZ, float farZ)
{
	float yScale = cosf(fovY / 2.f) / sinf(fovY / 2.f);
	float xScale = yScale / aspect;

	return Matrix(
		xScale, 0.f, 0.f, 0.f,
		0.f, yScale, 0.f, 0.f,
		0.f, 0.f, farZ / (farZ - nearZ), 1.f,
		0.f, 0.f, -nearZ * farZ / (farZ - nearZ), 0.f
		);
}

Matrix LookAtLH(const Vec3 & pos, const Vec3 & lookAt, const Vec3 & up)
{
	/**
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1
	*/
	Vec3 zaxis = (lookAt - pos).Normal();
	Vec3 xaxis = Vec3::Cross(up, zaxis).Normal();
	Vec3 yaxis = Vec3::Cross(zaxis, xaxis);

	return Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0.f,
		xaxis.y, yaxis.y, zaxis.y, 0.f,
		xaxis.z, yaxis.z, zaxis.z, 0.f,
		-Vec3::Dot(xaxis, pos), -Vec3::Dot(yaxis, pos), -Vec3::Dot(zaxis, pos), 1.f);
}

float Radians(float angle)
{
	return angle * PI / 180.f;
}

float Degrees(float radians)
{
	return radians * 180.f / PI;
}

};