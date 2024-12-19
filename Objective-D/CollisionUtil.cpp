#include "CollisionUtil.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"
#include "TransformUtil.h"
#include "GameResource.h"
#include "Config.h"

// �浹 ó���� ����ϴ� ��ƿ�̴�.
// ���� �ٸ� ������ �ٿ�� ��ü�͵� �� �����ϸ�, ��ü�� ������ ��ġ, ȸ��, ũ�⸦ �Ķ���Ϳ� �־��ָ� �ȴ�.
void OOBB::Update(Mesh* MeshPtr, XMFLOAT4X4& TMatrix, XMFLOAT4X4& RMatrix, XMFLOAT4X4& SMatrix) {
	if (MeshPtr) {
		XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMatrix), XMLoadFloat4x4(&RMatrix));
		ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMatrix));
		MeshPtr->OOBB.Transform(oobb, ResultMatrix);
		XMStoreFloat4(&oobb.Orientation, XMQuaternionNormalize(XMLoadFloat4(&oobb.Orientation)));
	}
}

void OOBB::Render(ID3D12GraphicsCommandList* CmdList) {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();

	Transform::Move(TranslateMatrix, oobb.Center.x, oobb.Center.y, oobb.Center.z);
	Transform::Scale(ScaleMatrix, oobb.Extents.x, oobb.Extents.y, oobb.Extents.z);

	// ���ʹϾ��� ȸ����ķ� ��ȯ
	XMVECTOR QuaternionForMatrix = XMLoadFloat4(&oobb.Orientation);
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(QuaternionForMatrix);
	XMStoreFloat4x4(&RotateMatrix, rotationMatrix);

	CBVUtil::Input(CmdList, BoolLightCBV, 0);
	CBVUtil::Input(CmdList, FlipCBV, 0);

	LineTex->Render(CmdList);
	BoundboxShader->Render(CmdList);

	camera.InitMatrix();
	camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f);
	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);

	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	float AlphaValue = 1.0;

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &BoundboxColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &AlphaValue, ALPHA_INDEX, 1, 0);

	BoundMesh->Render(CmdList);
#endif
}

bool OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollision(const BoundingBox& Other) {
	if (oobb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollision(const BoundingSphere& Other) {
	if (oobb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}



void AABB::Update(XMFLOAT3 Position, XMFLOAT3 Size) {
	aabb.Center = Position;
	aabb.Extents = Size;
}

void AABB::Render(ID3D12GraphicsCommandList* CmdList) {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();

	Transform::Move(TranslateMatrix, aabb.Center.x, aabb.Center.y, aabb.Center.z);
	Transform::Scale(ScaleMatrix, aabb.Extents.x, aabb.Extents.y, aabb.Extents.z);

	CBVUtil::Input(CmdList, BoolLightCBV, 0);
	CBVUtil::Input(CmdList, FlipCBV, 0);

	LineTex->Render(CmdList);
	BoundboxShader->Render(CmdList);

	camera.InitMatrix();
	camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f);
	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);

	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	float AlphaValue = 1.0;

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &BoundboxColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &AlphaValue, ALPHA_INDEX, 1, 0);

	BoundMesh->Render(CmdList);
#endif
}

bool AABB::CheckCollision(const AABB& Other) {
	if (aabb.Intersects(Other.aabb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollision(const BoundingOrientedBox& Other) {
	if (aabb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollision(const BoundingSphere& Other) {
	if (aabb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}



void Range::Update(const XMFLOAT3& Center, float Size) {
	sphere.Center = Center;
	sphere.Radius = Size;
}

bool Range::CheckCollision(const Range& Other) {
	if (sphere.Intersects(Other.sphere)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool Range::CheckCollision(const BoundingBox& Other) {
	if (sphere.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool Range::CheckCollision(const BoundingOrientedBox& Other) {
	if (sphere.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

void Range::Render(ID3D12GraphicsCommandList* CmdList) {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();

	Transform::Move(TranslateMatrix, sphere.Center.x, sphere.Center.y, sphere.Center.z);
	Transform::Scale(ScaleMatrix, sphere.Radius, sphere.Radius, sphere.Radius);

	CBVUtil::Input(CmdList, BoolLightCBV, 0);
	CBVUtil::Input(CmdList, FlipCBV, 0);

	LineTex->Render(CmdList);
	ObjectShader->Render(CmdList);

	camera.InitMatrix();
	camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f);
	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);

	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	float AlphaValue = 1.0;

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &BoundboxColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &AlphaValue, ALPHA_INDEX, 1, 0);

	BoundingSphereMesh->Render(CmdList);
#endif
}