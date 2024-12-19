#include "TransformUtil.h"

// ��ȯ�� ����ϴ� ��ƿ�̴�
// ������ �� ���� ��Ŀ� ���� Move, Rotate, Scale�� �����ϰ� �ϵ��� �Ϸ� ������
// ��ȯ�� ���� Ŀ���� �� �� �ֵ��� ����� ���۷����� �����ϵ��� �ߴ�

// �����̵� ��ȯ
void Transform::Move(XMFLOAT4X4& Matrix, float X, float Y, float Z) {
	XMMATRIX TranslateMat = XMMatrixTranslation(X, Y, Z);
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