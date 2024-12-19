#include "MathUtil.h"
#include <cmath>

// ���� ������ ���� �Լ����̴�. ��ü�� ������ ���� �������� ȸ������ �Ķ���Ϳ� �־��ָ� �ȴ�.

// ���͸� �⺻ ������ �ʱ�ȭ �Ѵ�.
void Math::InitVector(ObjectVector& VectorStruct) {
	VectorStruct.Up = XMFLOAT3(0.0, 1.0, 0.0);
	VectorStruct.Right = XMFLOAT3(1.0, 0.0, 0.0);
	VectorStruct.Look = XMFLOAT3(0.0, 0.0, 1.0);
}

// ������Ʈ�� ȸ���� ���� ���͸� ������Ʈ �Ѵ�. ī�޶� ������ �����ϱ� ���ؼ��� �ݵ�� �� �Լ��� ���͸� ������Ʈ �ؾ��Ѵ�.
void Math::UpdateVector(ObjectVector& VectorStruct, float Pitch, float Yaw, float Roll) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));

	VectorStruct.Up = {
		XMVectorGetX(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(UpVector, RotateMat))
	};

	VectorStruct.Look = {
		XMVectorGetX(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(LookVector, RotateMat))
	};

	VectorStruct.Right = {
		XMVectorGetX(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(RightVector, RotateMat))
	};
}

void Math::UpdateVector(ObjectVector& VectorStruct, XMFLOAT3& Rotation) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));

	VectorStruct.Up = {
		XMVectorGetX(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(UpVector, RotateMat))
	};

	VectorStruct.Look = {
		XMVectorGetX(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(LookVector, RotateMat))
	};

	VectorStruct.Right = {
		XMVectorGetX(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(RightVector, RotateMat))
	};
}

// ���͸� ����Ͽ� Ư�� ��ġ�� �ٶ󺸰� �ϵ��� �����Ѵ�. �ڽ��� ���, ����, ��ġ�� ����� ��ġ, �����͸� �Ķ���Ϳ� ������ �ȴ�.
void Math::LookAt(XMFLOAT4X4& Matrix, ObjectVector& VectorStruct, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition, XMFLOAT3& TargetUpVector) {
	XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(TargetPosition, ThisPosition, TargetUpVector);
	Matrix._11 = xmf4x4View._11; Matrix._12 = xmf4x4View._21; Matrix._13 = xmf4x4View._31;
	Matrix._21 = xmf4x4View._12; Matrix._22 = xmf4x4View._22; Matrix._23 = xmf4x4View._32;
	Matrix._31 = xmf4x4View._13; Matrix._32 = xmf4x4View._23; Matrix._33 = xmf4x4View._33;

	VectorStruct.Up = Vec3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23));
	VectorStruct.Right = Vec3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13));
	VectorStruct.Look = Vec3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33));
}

// ������� LookAt �Լ�
void Math::BillboardLookAt(XMFLOAT4X4& Matrix, ObjectVector& VectorStruct, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition) {
	XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(TargetPosition, ThisPosition, XMFLOAT3(0.0, 1.0, 0.0));
	Matrix._11 = xmf4x4View._11; Matrix._12 = xmf4x4View._21; Matrix._13 = xmf4x4View._31;

	VectorStruct.Up = Vec3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23));
	VectorStruct.Right = Vec3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13));
	VectorStruct.Look = Vec3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33));
}

// ȸ�������� ���̸� ����Ѵ�.
XMVECTOR Math::CalcRayDirection(XMFLOAT3& Rotation) {
	float RotationX = XMConvertToRadians(Rotation.x);
	float RotationY = XMConvertToRadians(Rotation.y);
	float RotationZ = XMConvertToRadians(Rotation.z);

	XMVECTOR RotationQuat = XMQuaternionRotationRollPitchYaw(RotationX, RotationY, RotationZ);
	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RayDirection = XMVector3Rotate(DefaultForward, RotationQuat);

	return RayDirection;
}

// ���̰� ���۵Ǵ� ��ġ�� ����Ѵ�.
XMVECTOR Math::CalcRayOrigin(XMFLOAT3& Position) {
	return XMVectorSet(Position.x, Position.y, Position.z, 1.0f);
}

// ���̰� �ٿ�� �ڽ��� �浹�ϴ��� �˻��Ѵ�
bool Math::CheckRayCollision(XMVECTOR& RayOrigin, XMVECTOR& RayDirection, const AABB& Other) {
	float Distance;
	return Other.aabb.Intersects(RayOrigin, RayDirection, Distance);
}

// ���̰� �ٿ�� �ڽ��� �浹�ϴ��� �˻��Ѵ�
bool Math::CheckRayCollision(XMVECTOR& RayOrigin, XMVECTOR& RayDirection, const OOBB& Other) {
	float Distance;
	return Other.oobb.Intersects(RayOrigin, RayDirection, Distance);
}

// ���̰� �ٿ�� �ڽ��� �浹�ϴ��� �˻��Ѵ�
bool Math::CheckRayCollision(XMVECTOR& RayOrigin, XMVECTOR& RayDirection, const Range& Other) {
	float Distance;
	return Other.sphere.Intersects(RayOrigin, RayDirection, Distance);
}

// ���� ������ �������� �ڽ��� ��ġ�� Ư�� �������� ���� �����ʿ� �ִ��� �˻��Ѵ�. �������̶�� true, �����̶�� false�� �����Ѵ�.
bool Math::IsRightOfTarget(XMFLOAT3& ThisPosition, ObjectVector& Vector, XMFLOAT3& TargetPosition) {
	XMFLOAT3 Direction = XMFLOAT3(
		TargetPosition.x - ThisPosition.x,
		TargetPosition.y - ThisPosition.y,
		TargetPosition.z - ThisPosition.z
	);

	XMVECTOR RightVec = XMLoadFloat3(&Vector.Right);
	XMVECTOR DirectionVec = XMLoadFloat3(&Direction);
	float DotProduct = XMVectorGetX(XMVector3Dot(RightVec, DirectionVec));

	if (DotProduct < 0)
		return true;

	return false;
}

// ��ġ�� ������ �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� look����, �ӵ����� �־��ָ� �ȴ�.
void Math::Vector_MoveForward(XMFLOAT3& Position, XMFLOAT3& Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

// ��ġ�� ������ �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� right����, �ӵ����� �־��ָ� �ȴ�.
void Math::Vector_MoveStrafe(XMFLOAT3& Position, XMFLOAT3& Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

// ��ġ�� ���� �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� up����, �ӵ����� �־��ָ� �ȴ�.
void Math::Vector_MoveUp(XMFLOAT3& Position, XMFLOAT3& Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

// ���� �������� ������ �����δ�.
void Math::MoveForward(XMFLOAT3& Position, float RotationY, float MoveDistance) {
	Position.x += sin(RotationY) * MoveDistance;
	Position.z += cos(RotationY) * MoveDistance;
}

// ���� �������� ������ �����δ�.
void Math::MoveStrafe(XMFLOAT3& Position, float RotationY, float MoveDistance) {
	Position.x += cos(RotationY) * MoveDistance;
	Position.z -= sin(RotationY) * MoveDistance;
}

// ���� �������� ���� �����δ�.
void Math::MoveUp(XMFLOAT3& Position, float MoveDistance) {
	Position.y += MoveDistance;
}

// 2���� �Ÿ��� ����Ѵ�.
float Math::CalcDistance2D(float FromX, float FromY, float ToX, float ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

// 2���� ������ ����Ѵ�.
float Math::CalcDegree2D(float FromX, float FromY, float ToX, float ToY) {
	return XMConvertToDegrees(atan2(ToY - FromY, ToX - FromX));
}

// 2���� ������ ����Ѵ�.
float Math::CalcRadians2D(float FromX, float FromY, float ToX, float ToY) {
	return atan2(ToY - FromY, ToX - FromX);
}

// 2���� ������ 360�� ������ ����ȭ �Ѵ�.
void Math::Normalize2DAngleTo360(float& Degree) {
	Degree = fmod(Degree, 360.0f);
	if (Degree < 0.0f)
		Degree += 360.0f;
}
