#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// GameObject Ŭ������ ��� ��ü���� ��ӹ޴� �θ� Ŭ�����̴�.
// ��� ��ü�� �ݵ�� �� Ŭ�����κ��� �󺹹޾ƾ� �����ӿ�ũ�� ��ü�� ������Ʈ�ϰ� �������Ѵ�.
// �Ϻ� �Լ����� ������ ���Ϸ� �и� �����̴� �ڵ忡 ������ ���� �� �ִ�.

// ��ü�� ����� �ʱ�ȭ �Ѵ�. ��� ��ü�� ��ȯ �۾� �� �ݵ�� �� �Լ��� ù ��°�� �����ؾ� �Ѵ�.
void GameObject::InitMatrix(ID3D12GraphicsCommandList* CmdList, int RenderTypeFlag) {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();

	// ��� ��� ����
	RenderType = RenderTypeFlag;

	// �ؽ�ó ���� �ʱ�ȭ
	ObjectAlpha = 1.0f;
	FlipTexture(CmdList, FLIP_TYPE_NONE);

	// �Ž� ���� �ʱ�ȭ
	SetColor(0.0, 0.0, 0.0);

	// �̹��� Ÿ�� �������� ��� �̹��� ���� ����
	if (RenderTypeFlag == RENDER_TYPE_IMAGE)
		SetToImageMode(CmdList);

	else // ������ Ÿ���� ��� �⺻ ���� ����
		SetToDefaultMode(CmdList);
}

// ��ü �޽��� ������ �����Ѵ�.
void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

// ��ü �Ž��� ������ �����Ѵ�.
void GameObject::SetColor(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

//  ��ü�� ������ �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� look����, �ӵ����� �־��ָ� �ȴ�.
void GameObject::MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

//  ��ü�� ������ �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� right����, �ӵ����� �־��ָ� �ȴ�.
void GameObject::MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

// ��ü�� ���� �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� up����, �ӵ����� �־��ָ� �ȴ�.
void GameObject::MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

// �ؽ�ó�� ������Ų��. �𵨿� ���� �ٸ��� ����� �� �ִ�.
void GameObject::FlipTexture(ID3D12GraphicsCommandList* CmdList, int FlipType) {
	CBVUtil::Input(CmdList, FlipCBV, FlipType);
}

// ���� ��� ��Ȱ��ȭ
void GameObject::DisableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, BoolLightCBV, 0);
}

// ���� ��� Ȱ��ȭ
void GameObject::EnableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, BoolLightCBV, 1);
}

// ���̴��� ���� ������ ����
void GameObject::InputLightInfo(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, LightCBV, 0);
}

// ���� ��Ⱦ�� ���Ѵ�. UI�� �����Ҷ� �ַ� ����Ѵ�.
float GameObject::ASP(float Value) {
	return ASPECT * Value;
}

void GameObject::UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY) {
	RotationX += DeltaY;
	RotationY += DeltaX;
}

void GameObject::UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY) {
	Rotation.x += DeltaY;
	Rotation.y += DeltaX;
}

////////////////////////////////
// 
// �ؽ�ó�� ���ε��Ѵ�. �ݵ�� �Ž��� �������ϱ� ���� ����ؾ� �Ѵ�. Ŀ�ǵ� ����Ʈ�� ������ �ؽ�ó�� �Ķ���ͷ� �����ϸ� �ȴ�.
// �¿� �Ǵ� ���� �Ǵ� �¿���� ������ �����ϴ�.
void GameObject::BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr) {
	if (TexturePtr)
		TexturePtr->Render(CmdList);
}

// ���̴��� �����Ѵ�. �ݵ�� �Ž��� �������ϱ� ���� ����ؾ� �Ѵ�. Ŀ�ǵ� ����Ʈ�� ������ ���̴��� �Ķ���ͷ� �����ϸ� �ȴ�.
void GameObject::UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest) {
	if (ShaderPtr)
		ShaderPtr->Render(CmdList, DepthTest);
}

// �޽��� ������ �Ѵ�. ��ȯ �۾��� ���� �� �� �������� �����Ѵ�.
void GameObject::RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr, Texture* TexturePtr, Shader* ShaderPtr, float Alpha, bool DepthTestFlag) {
	// �ؽ�ó ���ε�
	BindTexture(CmdList, TexturePtr);

	// �̹��� ���� Ÿ���� �ƴ϶�� ���� �˻� �÷��׸� �������� �Ѵ�.
	if(RenderType != RENDER_TYPE_IMAGE)
		UseShader(CmdList, ShaderPtr, DepthTestFlag);
	else 
		UseShader(CmdList, ShaderPtr, false);

	// ī�޶� ����� �ʱ�ȭ �Ѵ�
	camera.InitMatrix();

	if (RenderType == RENDER_TYPE_PERS)
		camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f);

	else if (RenderType == RENDER_TYPE_ORTHO || RenderType == RENDER_TYPE_IMAGE)
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0f, 10.0f);

	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);

	ObjectAlpha = Alpha;

	// ���� ������ ����¡ ���� ���̴��� �����Ѵ�.
	// �̹��� ����� ��� ������ ��Ȱ��ȭ �ȴ�.
	InputLightInfo(CmdList);
	UpdateShaderVariables(CmdList);
	if (MeshPtr)
		MeshPtr->Render(CmdList);
}

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;

	XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
	GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
	nIntersected = MeshPtr->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);

	return(nIntersected);
}

// ��İ� ���̴� �� ���� ���� ������ ���̴��� �����Ѵ�. RenderMesh�Լ��� �����ϸ� �� �Լ��� ����ȴ�. ��, ���� ����� ���� ����.
void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &ModelColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &ObjectAlpha, ALPHA_INDEX, 1, 0);
}

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
void GameObject::GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, ResultMatrix * xmmtxView);
	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

// �̹��� ��� ���� ��ȯ�Ѵ�. �ؽ�ó ���� ���� �� ���� ����� ��Ȱ��ȭ �Ѵ�.
void GameObject::SetToImageMode(ID3D12GraphicsCommandList* CmdList) {
	FlipTexture(CmdList, FLIP_TYPE_V);
	DisableLight(CmdList);
	camera.SetToStaticMode();
}

// �⺻ ��� ���� ��ȯ�Ѵ�. �ؽ�ó ���� ���� �� ���� ����� Ȱ��ȭ �Ѵ�.
void GameObject::SetToDefaultMode(ID3D12GraphicsCommandList* CmdList) {
	FlipTexture(CmdList, FLIP_TYPE_NONE);
	EnableLight(CmdList);
	camera.SetToDefaultMode();
}