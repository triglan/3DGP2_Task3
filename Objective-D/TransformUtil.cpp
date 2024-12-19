#include "TransformUtil.h"

// ��ȯ�� ����ϴ� ��ƿ�̴�
// ������ �� ���� ��Ŀ� ���� Move, Rotate, Scale�� �����ϰ� �ϵ��� �Ϸ� ������
// ��ȯ�� ���� Ŀ���� �� �� �ֵ��� ����� ���۷����� �����ϵ��� �ߴ�

// ��� �ʱ�ȭ
void Transform::Identity(XMFLOAT4X4& Matrix) {
	Matrix = Mat4::Identity();
}

// �����̵� ��ȯ
void Transform::Move(XMFLOAT4X4& Matrix, float X, float Y, float Z) {
	XMMATRIX TranslateMat = XMMatrixTranslation(X, Y, Z);
	Matrix = Mat4::Multiply(TranslateMat, Matrix);
}

// �����̵� ��ȯ
void Transform::Move(XMFLOAT4X4& Matrix, XMFLOAT3& Value) {
	XMMATRIX TranslateMat = XMMatrixTranslation(Value.x,Value.y, Value.z);
	Matrix = Mat4::Multiply(TranslateMat, Matrix);
}

// ���� �����Ͽ� Ư�� ������ŭ ȸ�� ��ȯ ��Ų��.
void Transform::Rotate(XMFLOAT4X4& Matrix, float RotationValue, float X, float Y, float Z) {
	XMMATRIX RotateMat = XMMatrixRotationAxis(XMVectorSet(X, Y, Z, 0.0f), XMConvertToRadians(RotationValue));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// �� �࿡ ȸ�� ������ �ο��Ͽ� ȸ�� ��ȯ ��Ų��.
void Transform::Rotate(XMFLOAT4X4& Matrix, float RotationX, float RotationY, float RotationZ) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(RotationX), XMConvertToRadians(RotationY), XMConvertToRadians(RotationZ));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// �� �࿡ ȸ�� ������ �ο��Ͽ� ȸ�� ��ȯ ��Ų��.
void Transform::Rotate(XMFLOAT4X4& Matrix, XMFLOAT3& Rotation) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// Y�� ȸ�� �� X�� ȸ�� ��ȯ
void Transform::RotateYX(XMFLOAT4X4& Matrix, float RotationX, float RotationY) {
	XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(-RotationY));
	XMMATRIX rotationX = XMMatrixRotationX(XMConvertToRadians(-RotationX));

	XMMATRIX rotationMat = rotationY * rotationX;

	XMMATRIX currentMat = XMLoadFloat4x4(&Matrix);
	XMMATRIX finalMat = rotationMat * currentMat;

	// ����� XMFLOAT4X4�� �ٽ� ����
	XMStoreFloat4x4(&Matrix, finalMat);
}

// ������ ��ȯ
void Transform::Scale(XMFLOAT4X4& Matrix, float ScaleX, float ScaleY, float ScaleZ) {
	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}

// ������ ��ȯ
void Transform::Scale(XMFLOAT4X4& Matrix, XMFLOAT3& Size) {
	XMMATRIX ScaleMat = XMMatrixScaling(Size.x, Size.y, Size.z);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}

// �̹��� ��� �� �Ž��� ��Ⱦ��� �̹��� ��Ⱦ�� �����ϰ� �����.
// ��ȯ�� ��İ� �̹����� ũ�⸦ �Ķ���Ϳ� �����ϸ� �ȴ�.
void Transform::ImageAspect(XMFLOAT4X4& Matrix, int Width, int Height) {
	float ScaleX{}, ScaleY{};

	if (Width > Height) {
		ScaleX = 1.0;
		ScaleY = (float)Height / (float)Width;
	}

	else if (Width < Height) {
		ScaleX = (float)Width / (float)Height;
		ScaleY = 1.0;
	}

	else {
		ScaleX = 1.0;
		ScaleY = 1.0;
	}

	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, 1.0);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}

// 2���� �̵� ��ȯ
void Transform::Move2D(XMFLOAT4X4& Matrix, float X, float Y) {
	XMMATRIX TranslateMat = XMMatrixTranslation(X, Y, 0.0);
	Matrix = Mat4::Multiply(TranslateMat, Matrix);
}

// 2���� ȸ�� ��ȯ
void Transform::Rotate2D(XMFLOAT4X4& Matrix, float Rotation) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.0), XMConvertToRadians(0.0), XMConvertToRadians(Rotation));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// 2���� ������ ��ȯ
void Transform::Scale2D(XMFLOAT4X4& Matrix, float X, float Y) {
	XMMATRIX ScaleMat = XMMatrixScaling(X, Y, 1.0);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}