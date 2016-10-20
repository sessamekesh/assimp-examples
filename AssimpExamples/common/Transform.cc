#include <Transform.h>

namespace sess
{

const Transform Transform::Identity = Transform();

Transform::Transform()
	: Position(0.f, 0.f, 0.f)
	, Rotation()
	, Scale(1.f, 1.f, 1.f)
{}

Transform::Transform(const Vec3& pos, const Quaternion& rotation, const Vec3& scale)
	: Position(pos)
	, Rotation(rotation)
	, Scale(scale)
{}

Transform Transform::operator*(const Transform& o) const
{
	// Apply first this transformation, and then the next transformation.
	return Transform(
		Position + Vec3::ComponentProduct(o.Position * Rotation, Scale),
		Rotation * o.Rotation,
		Vec3::ComponentProduct(Scale, o.Scale)
		);
}

Matrix Transform::GetTransformMatrix() const
{
	Matrix m;

	m._11 = Scale.x * (1.f - 2.f * Rotation.y * Rotation.y - 2.f * Rotation.z * Rotation.z);
	m._12 = Scale.y * (2.f * Rotation.x * Rotation.y - 2.f * Rotation.z * Rotation.w);
	m._13 = Scale.z * (2.f * Rotation.x * Rotation.z + 2.f * Rotation.y * Rotation.w);
	m._14 = Position.x;

	m._21 = Scale.x * (2.f * Rotation.x * Rotation.y + 2.f * Rotation.z * Rotation.w);
	m._22 = Scale.y * (1.f - 2.f * Rotation.x * Rotation.x - 2.f * Rotation.z * Rotation.z);
	m._23 = Scale.z * (2.f * Rotation.y * Rotation.z - 2.f * Rotation.x * Rotation.w);
	m._24 = Position.y;

	m._31 = Scale.x * (2.f * Rotation.x * Rotation.z - 2.f * Rotation.y * Rotation.w);
	m._32 = Scale.y * (2.f * Rotation.y * Rotation.z + 2.f * Rotation.x * Rotation.w);
	m._33 = Scale.z * (1.f - 2.f * Rotation.x * Rotation.x - 2.f * Rotation.y * Rotation.y);
	m._34 = Position.z;

	m._41 = 0.f;
	m._42 = 0.f;
	m._43 = 0.f;
	m._44 = 1.f;

	return m;
}

Transform Transform::Inverse() const
{
	return Transform({ -Position, Rotation.Inverse(), Vec3(1.f / Scale.x, 1.f / Scale.y, 1.f / Scale.z) });
}

Transform Transform::Lerp(const Transform& t1, const Transform& t2, float ratio)
{
	// TODO SESS: Might not be accurate enough, use Slerp for quaternions
	return Transform(
		t1.Position * (1.f - ratio) + t2.Position * ratio,
		Quaternion(
			t1.Rotation.w * (1.f - ratio) + t2.Rotation.w * ratio,
			t1.Rotation.x * (1.f - ratio) + t2.Rotation.x * ratio,
			t1.Rotation.y * (1.f - ratio) + t2.Rotation.y * ratio,
			t1.Rotation.z * (1.f - ratio) + t2.Rotation.z * ratio
			),
		t1.Scale * (1.f - ratio) + t2.Scale * ratio
		);
}

Transform Transform::FromTransformMatrix(const Matrix& m)
{
	Vec3 pos(m._14, m._24, m._34);

	// To find rotation, perform a rotation on a special vector.
	// Where M is the transformation matrix, R is the rotation
	//  and S is the scale
	// t = {1, 1, 1}
	// t' = Mt = RSt
	// t x t' = a (axis of rotation)
	// norm(t) * norm(t') = cos(theta) (angle of rotation)
	//  R can be composed from axis and angle of rotation
	// RSt = t'
	// St = R^(-1)t'

	float sx = Vec3(m._11, m._21, m._31).Magnitude();
	float sy = Vec3(m._12, m._22, m._32).Magnitude();
	float sz = Vec3(m._13, m._23, m._33).Magnitude();

	// http://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
	Quaternion rotation = Quaternion::FromMatrix(m);

	Vec3 scale = Vec3(sx, sy, sz);

	return Transform(pos, rotation, scale);
}

};