#include "CameraUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"
#include "TransformUtil.h"

// Config.h ���� �ۼ��� ��忡 ���� ī�޶� �ٸ��� �����ϵ��� �ۼ��� �� �ִ�.
// ��) ī�޶� ���� ��� ����, ī�޶� ���� ���� ��
void Camera::Update(float FT) {
	switch (Mode) {
	case CamMode::MODE1:
		break;
	}
}

// ī�޶� ��带 �����Ѵ�. Config.h�� �ۼ��ߴ� ��� �������� �Ķ���Ϳ� ������ �ȴ�.
void Camera::SwitchCameraMode(CamMode ModeValue) {
	Mode = ModeValue;
}

// ���� ���� ���� ī�޶� ��带 ��´�.
CamMode Camera::CurrentMode() {
	return Mode;
}

// ���� ��� ���� ��ȯ�Ѵ�.
void Camera::SetToStaticMode() {
	StaticMode = true;
}

// �Ϲ� ��� ���� ��ȯ�Ѵ�.
void Camera::SetToDefaultMode() {
	StaticMode = false;
}



Camera::Camera() {
	ViewMatrix = Mat4::Identity();
	ProjectionMatrix = Mat4::Identity();

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Pitch = 0.0f;
	Roll = 0.0f;
	Yaw = 0.0f;

	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	LookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);

	MovingDelay = 0.0f;
}



void Camera::UpdateShaderVariables() {
	XMFLOAT4X4 xmf4x4View;
	XMFLOAT4X4 xmf4x4Projection;

	// ����ƽ ��� ���� �� ����ƽ ����� ���̴��� �����Ѵ�.
	switch(StaticMode) {
	case false:
		XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&ViewMatrix)));
		XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&ProjectionMatrix)));
		RCUtil::Input(ObjectCmdList, &Position, CAMERA_INDEX, 3, 32);
		break;

	case true:
		XMFLOAT3 StaticPosition{ 0.0, 0.0, 0.0 };
		XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&StaticViewMatrix)));
		XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&StaticProjectionMatrix)));
		RCUtil::Input(ObjectCmdList, &StaticPosition, CAMERA_INDEX, 3, 32);
	}

	RCUtil::Input(ObjectCmdList, &xmf4x4View, CAMERA_INDEX, 16, 0);
	RCUtil::Input(ObjectCmdList, &xmf4x4Projection, CAMERA_INDEX, 16, 16);
}

// ī�޶� �� ����� �����Ѵ�.
void Camera::SetViewMatrix() {
	Look = Vec3::Normalize(Look);
	Right = Vec3::CrossProduct(Up, Look, true);
	Up = Vec3::CrossProduct(Look, Right, true);

	ViewMatrix._11 = Right.x; ViewMatrix._12 = Up.x; ViewMatrix._13 = Look.x;
	ViewMatrix._21 = Right.y; ViewMatrix._22 = Up.y; ViewMatrix._23 = Look.y;
	ViewMatrix._31 = Right.z; ViewMatrix._32 = Up.z; ViewMatrix._33 = Look.z;

	ViewMatrix._41 = -Vec3::DotProduct(Position, Right);
	ViewMatrix._42 = -Vec3::DotProduct(Position, Up);
	ViewMatrix._43 = -Vec3::DotProduct(Position, Look);

	InverseView._11 = Right.x; InverseView._12 = Right.y; InverseView._13 = Right.z;
	InverseView._21 = Up.x; InverseView._22 = Up.y; InverseView._23 = Up.z;
	InverseView._31 = Look.x; InverseView._32 = Look.y; InverseView._33 = Look.z;
	InverseView._41 = Position.x; InverseView._42 = Position.y; InverseView._43 = Position.z;

	FrustumView.Transform(FrustumWorld, XMLoadFloat4x4(&InverseView));
}

// ���� ���� ����� �ʱ�ȭ�Ѵ�. ������ ������ ���� �� �� �Լ��� ����ȴ�.
void Camera::GeneratePerspectiveMatrix(float NearPlane, float FarPlane, float AspRatio, float Fov) {
	Transform::Identity(ProjectionMatrix);
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(Fov), AspRatio, NearPlane, FarPlane);
	XMStoreFloat4x4(&ProjectionMatrix, Projection);

#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	BoundingFrustum::CreateFromMatrix(FrustumView, Projection);
#endif
}

// ���� ���� ����� �ʱ�ȭ �Ѵ�
void Camera::GenerateOrthoMatrix(float Width, float Height, float AspRatio, float Near, float Far) {
	Transform::Identity(ProjectionMatrix);
	XMMATRIX Projection = XMMatrixOrthographicLH(Width * AspRatio * 2.0, Height * 2.0, Near, Far);
	XMStoreFloat4x4(&ProjectionMatrix, Projection);

#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	BoundingFrustum::CreateFromMatrix(FrustumView, Projection);
#endif
}

// ���� ���� ���� ����� �ʱ�ȭ �Ѵ�
void Camera::GenerateStaticMatrix() {
	Transform::Identity(StaticProjectionMatrix);
	XMMATRIX Projection = XMMatrixOrthographicLH(2.0 * ASPECT, 2.0, 0.0, 10.0);
	XMStoreFloat4x4(&StaticProjectionMatrix, Projection);
}

// ���� ����� ���� ����ƽ ����� �����Ѵ�. UI, �̹��� ���� ����� �������� �ϴ� ����̹Ƿ� 
// ���α׷� ���� �� ���� 1ȸ�� �����Ѵ�.
// ���������� ����Ʈ�̴�.
void Camera::InitStaticMatrix() {
	Transform::Identity(StaticViewMatrix);
	Transform::Identity(StaticProjectionMatrix);
	XMMATRIX Projection = XMMatrixOrthographicLH(2.0 * ASPECT, 2.0, 0.0, 10.0);
	XMStoreFloat4x4(&StaticProjectionMatrix, Projection);
}

// ����Ʈ�� �����Ѵ�. �� �� ������ ���Ŀ��� �ǵ� �ʿ� ����.
void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float zMin, float zMax) {
	Viewport.TopLeftX = float(xTopLeft);
	Viewport.TopLeftY = float(yTopLeft);
	Viewport.Width = float(nWidth);
	Viewport.Height = float(nHeight);
	Viewport.MinDepth = zMin;
	Viewport.MaxDepth = zMax;
}

// ���� ��Ʈ�� �����Ѵ�. �� �� ������ ���Ŀ��� �ǵ� �ʿ� ����.
void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom) {
	ScissorRect.left = xLeft;
	ScissorRect.top = yTop;
	ScissorRect.right = xRight;
	ScissorRect.bottom = yBottom;
}

// ����Ʈ�� ������Ʈ�� ���̴��� �����Ѵ�.
void Camera::SetViewportsAndScissorRects() {
	ObjectCmdList->RSSetViewports(1, &Viewport);
	ObjectCmdList->RSSetScissorRects(1, &ScissorRect);
}



// ��ġ �̵�, ���� ���� ��ġ ���� �� ȸ������, ��ġ, ���� ���� �Լ����̴�.
XMFLOAT3& Camera::GetPosition() { 
	return Position; 
}

void Camera::SetLookAtPosition(XMFLOAT3 LookAtValue) { 
	LookAt = LookAtValue; 
}

XMFLOAT3& Camera::GetLookAtPosition() { 
	return LookAt; 
}

XMFLOAT3& Camera::GetRightVector() { 
	return Right; 
}

XMFLOAT3& Camera::GetUpVector() {
	return Up; 
}

XMFLOAT3& Camera::GetLookVector() { 
	return Look; 
}

float& Camera::GetPitch() { 
	return Pitch; 
}

float& Camera::GetRoll() {
	return Roll;
}

float& Camera::GetYaw() { 
	return Yaw;
}

void Camera::SetOffset(XMFLOAT3 Value) { 
	Offset = Value;
}

XMFLOAT3& Camera::GetOffset() { 
	return Offset;
}

void Camera::SetTimeLag(float DelayValue) { 
	MovingDelay = DelayValue;
}

float Camera::GetTimeLag() { 
	return MovingDelay;
}

XMFLOAT4X4 Camera::GetViewMatrix() { 
	return ViewMatrix; 
}

XMFLOAT4X4 Camera::GetProjectionMatrix() {
	return ProjectionMatrix; 
}

D3D12_VIEWPORT Camera::GetViewport() { 
	return Viewport; 
}

D3D12_RECT Camera::GetScissorRect() { 
	return ScissorRect; 
}



// ī�޶��� ��ġ�� �����Ѵ�.
void Camera::Move(float X, float Y, float Z) {
	Position.x = X;
	Position.y = Y;
	Position.z = Z;
}

// ī�޶��� ��ġ�� �����Ѵ�.
void Camera::Move(XMFLOAT3 PositionValue) { 
	Position = PositionValue; 
}

// ���� �������� look ���͸� ����Ͽ� ������ �����δ�.
void Camera::Vector_MoveForward(float MoveDistance) {
	XMFLOAT3 NormlaizedLook = Vec3::Normalize(Look);

	Position.x += NormlaizedLook.x * MoveDistance;
	Position.y += NormlaizedLook.y * MoveDistance;
	Position.z += NormlaizedLook.z * MoveDistance;
}

// ���� �������� right ���͸� ����Ͽ� ������ �����δ�.
void Camera::Vector_MoveStrafe(float MoveDistance) {
	XMFLOAT3 NormlaizedRight = Vec3::Normalize(Right);

	Position.x += NormlaizedRight.x * MoveDistance;
	Position.y += NormlaizedRight.y * MoveDistance;
	Position.z += NormlaizedRight.z * MoveDistance;
}

// ���� �������� up ���͸� ����Ͽ� ���� �����δ�.
void Camera::Vector_MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

// ���� �������� ������ �����δ�.
void Camera::MoveForward(float MoveDistance) {
	Position.x += sin(Yaw) * MoveDistance;
	Position.z += cos(Yaw) * MoveDistance;
}

// ���� �������� ������ �����δ�.
void Camera::MoveStrafe(float MoveDistance) {
	Position.x += cos(Yaw) * MoveDistance;
	Position.z -= sin(Yaw) * MoveDistance;
}

// �������� �����δ�.
void Camera::MoveUp(float MoveDistance) {
	Position.y += MoveDistance;
}

// ī�޶� ȸ��, ���ο� ���͸� �����Ѵ�.
void Camera::Rotate(float X, float Y, float Z) {
	Look = XMFLOAT3(0.0, 0.0, 1.0);
	Right = XMFLOAT3(1.0, 0.0, 0.0);
	Up = XMFLOAT3(0.0, 1.0, 0.0);

	Pitch = XMConvertToRadians(X);
	Yaw = XMConvertToRadians(Y);
	Roll = XMConvertToRadians(Z);

	// ȸ�� ��� ����
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	XMStoreFloat3(&Look, XMVector3TransformNormal(XMLoadFloat3(&Look), RotationMatrix));
	XMStoreFloat3(&Right, XMVector3TransformNormal(XMLoadFloat3(&Right), RotationMatrix));
	XMStoreFloat3(&Up, XMVector3TransformNormal(XMLoadFloat3(&Up), RotationMatrix));
}

// �Ķ���ͷ� �Է¹��� ��ġ, ������, ����Ʈ����, �躤�͸� �����Ѵ�.
void Camera::Track(XMFLOAT3& ObjectPosition, Vector& VectorStruct, float fTimeElapsed) {
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();

	XMFLOAT3 UpVector = VectorStruct.Up;
	XMFLOAT3 RightVector = VectorStruct.Right;
	XMFLOAT3 LookVector = VectorStruct.Look;

	RotateMatrix._21 = UpVector.x;
	RotateMatrix._22 = UpVector.y;
	RotateMatrix._23 = UpVector.z;

	RotateMatrix._11 = RightVector.x;
	RotateMatrix._12 = RightVector.y;
	RotateMatrix._13 = RightVector.z;

	RotateMatrix._31 = LookVector.x;
	RotateMatrix._32 = LookVector.y;
	RotateMatrix._33 = LookVector.z;

	XMFLOAT3 Direction = Vec3::Subtract(Vec3::Add(ObjectPosition, Vec3::TransformCoord(Offset, RotateMatrix)), Position);

	float Length = Vec3::Length(Direction);
	Direction = Vec3::Normalize(Direction);

	float TimeLagScale = (MovingDelay) ? fTimeElapsed * (1.0f / MovingDelay) : 1.0f;
	float MoveDistance = Length * TimeLagScale;

	if (MoveDistance > Length)
		MoveDistance = Length;

	if (Length < 0.01f)
		MoveDistance = Length;

	Position = Vec3::Add(Position, Direction, MoveDistance);
	SetLookAt(ObjectPosition, VectorStruct.Up);
}

// ������ Track�� �����ϳ�, ���� Offset�� ������ �� �ִ�.
void Camera::TrackOffset(XMFLOAT3& ObjectPosition, Vector& VectorStruct, XMFLOAT3& OffsetValue, float fTimeElapsed) {
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();

	XMFLOAT3 UpVector = VectorStruct.Up;
	XMFLOAT3 RightVector = VectorStruct.Right;
	XMFLOAT3 LookVector = VectorStruct.Look;

	RotateMatrix._21 = UpVector.x;
	RotateMatrix._22 = UpVector.y;
	RotateMatrix._23 = UpVector.z;

	RotateMatrix._11 = RightVector.x;
	RotateMatrix._12 = RightVector.y;
	RotateMatrix._13 = RightVector.z;

	RotateMatrix._31 = LookVector.x;
	RotateMatrix._32 = LookVector.y;
	RotateMatrix._33 = LookVector.z;

	XMFLOAT3 Direction = Vec3::Subtract(Vec3::Add(ObjectPosition, Vec3::TransformCoord(Offset, RotateMatrix)), Position);

	float Length = Vec3::Length(Direction);
	Direction = Vec3::Normalize(Direction);

	float TimeLagScale = (MovingDelay) ? fTimeElapsed * (1.0f / MovingDelay) : 1.0f;
	float MoveDistance = Length * TimeLagScale;

	if (MoveDistance > Length)
		MoveDistance = Length;

	if (Length < 0.01f)
		MoveDistance = Length;

	Position = Vec3::Add(Position, Direction, MoveDistance);

	// ���� ��ǥ�迡�� LookAtPosition ����
	XMFLOAT3 LookAtPosition = ObjectPosition;

	// ���� ��ǥ�踦 �������� ������ ����
	LookAtPosition = Vec3::Add(LookAtPosition, Vec3::Scale(VectorStruct.Right, OffsetValue.x));
	LookAtPosition = Vec3::Add(LookAtPosition, Vec3::Scale(VectorStruct.Up, OffsetValue.y));
	LookAtPosition = Vec3::Add(LookAtPosition, Vec3::Scale(VectorStruct.Look, OffsetValue.z));

	SetLookAt(LookAtPosition, VectorStruct.Up);
}

// ī�޶� �ٶ󺸴� ������ �����Ѵ�. Track���� ����ǹǷ� ������ ��� ���� �� ���� ����.
void Camera::SetLookAt(XMFLOAT3& ObjectPosition, XMFLOAT3& UpVec) {
	XMFLOAT4X4 mtxLookAt = Mat4::LookAtLH(Position, ObjectPosition, UpVec);
	Right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	Up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	Look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}




// �������� ���� �Լ���
void Camera::CalculateFrustumPlanes() {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	FrustumView.Transform(FrustumWorld, XMMatrixInverse(NULL, XMLoadFloat4x4(&ViewMatrix)));
#else
	XMFLOAT4X4 CamViewProjection = Mat4::Multiply(ViewMatrix, ProjectionMatrix);

	FrustumPlane[0].x = -(CamViewProjection._14 + CamViewProjection._11);
	FrustumPlane[0].y = -(CamViewProjection._24 + CamViewProjection._21);
	FrustumPlane[0].z = -(CamViewProjection._34 + CamViewProjection._31);
	FrustumPlane[0].w = -(CamViewProjection._44 + CamViewProjection._41);

	FrustumPlane[1].x = -(CamViewProjection._14 - CamViewProjection._11);
	FrustumPlane[1].y = -(CamViewProjection._24 - CamViewProjection._21);
	FrustumPlane[1].z = -(CamViewProjection._34 - CamViewProjection._31);
	FrustumPlane[1].w = -(CamViewProjection._44 - CamViewProjection._41);

	FrustumPlane[2].x = -(CamViewProjection._14 - CamViewProjection._12);
	FrustumPlane[2].y = -(CamViewProjection._24 - CamViewProjection._22);
	FrustumPlane[2].z = -(CamViewProjection._34 - CamViewProjection._32);
	FrustumPlane[2].w = -(CamViewProjection._44 - CamViewProjection._42);

	FrustumPlane[3].x = -(CamViewProjection._14 + CamViewProjection._12);
	FrustumPlane[3].y = -(CamViewProjection._24 + CamViewProjection._22);
	FrustumPlane[3].z = -(CamViewProjection._34 + CamViewProjection._32);
	FrustumPlane[3].w = -(CamViewProjection._44 + CamViewProjection._42);

	FrustumPlane[4].x = -(CamViewProjection._13);
	FrustumPlane[4].y = -(CamViewProjection._23);
	FrustumPlane[4].z = -(CamViewProjection._33);
	FrustumPlane[4].w = -(CamViewProjection._43);

	FrustumPlane[5].x = -(CamViewProjection._14 - CamViewProjection._13);
	FrustumPlane[5].y = -(CamViewProjection._24 - CamViewProjection._23);
	FrustumPlane[5].z = -(CamViewProjection._34 - CamViewProjection._33);
	FrustumPlane[5].w = -(CamViewProjection._44 - CamViewProjection._43);

	for (int i = 0; i < 6; i++)
		FrustumPlane[i] = Plane::Normalize(FrustumPlane[i]);
#endif
}

bool Camera::IsInFrustum(BoundingBox& BoundingBox) {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	return(FrustumWorld.Intersects(BoundingBox));
#else
	XMFLOAT3 NearPoint, Normal;
	XMFLOAT3 Min = Vec3::Subtract(BoundingBox.Center, BoundingBox.Extents);
	XMFLOAT3 Max = Vec3::Add(BoundingBox.Center, BoundingBox.Extents);
	for (int i = 0; i < 6; i++) {
		Normal = XMFLOAT3(FrustumPlane[i].x, FrustumPlane[i].y, FrustumPlane[i].z);

		if (Normal.x >= 0.0f) {
			if (Normal.y >= 0.0f) {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Min.x, Min.y, Min.z);
				else
					NearPoint = XMFLOAT3(Min.x, Min.y, Max.z);
			}

			else {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Min.x, Max.y, Min.z);
				else
					NearPoint = XMFLOAT3(Min.x, Max.y, Max.z);
			}
		}

		else {
			if (Normal.y >= 0.0f) {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Max.x, Min.y, Min.z);
				else
					NearPoint = XMFLOAT3(Max.x, Min.y, Max.z);
			}

			else {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Max.x, Max.y, Min.z);
				else
					NearPoint = XMFLOAT3(Max.x, Max.y, Max.z);
			}
		}

		if ((Vec3::DotProduct(Normal, NearPoint) + FrustumPlane[i].w) > 0.0f)
			return(false);
	}

	return(true);
#endif
}

bool Camera::IsInFrustum(BoundingOrientedBox& BoundingBox) {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	return(FrustumWorld.Intersects(BoundingBox));
#else
#endif
}