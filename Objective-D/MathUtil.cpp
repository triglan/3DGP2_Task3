#include "MathUtil.h"
#include <cmath>

// ���� ������ ���� �Լ����̴�. ��ü�� ������ ���� �������� ȸ������ �Ķ���Ϳ� �־��ָ� �ȴ�.

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

// ���͸� �ʱ�ȭ �Ѵ�.
void Math::InitVector(ObjectVector& VectorStruct) {
	VectorStruct.Up = XMFLOAT3(0.0, 1.0, 0.0);
	VectorStruct.Right = XMFLOAT3(1.0, 0.0, 0.0);
	VectorStruct.Look = XMFLOAT3(0.0, 0.0, 1.0);
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

float Math::CalcDistance2D(float FromX, float FromY, float ToX, float ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

float Math::CalcDegree2D(float FromX, float FromY, float ToX, float ToY) {
	return XMConvertToDegrees(atan2(ToY - FromY, ToX - FromX));
}

float Math::CalcRadians2D(float FromX, float FromY, float ToX, float ToY) {
	return atan2(ToY - FromY, ToX - FromX);
}

void Math::BillboardLookAt(XMFLOAT4X4& Matrix, ObjectVector& VectorStruct, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition) {
	XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(TargetPosition, ThisPosition, XMFLOAT3(0.0, 1.0, 0.0));
	Matrix._11 = xmf4x4View._11; Matrix._12 = xmf4x4View._21; Matrix._13 = xmf4x4View._31;

	VectorStruct.Up = Vec3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23));
	VectorStruct.Right = Vec3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13));
	VectorStruct.Look = Vec3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33));
}