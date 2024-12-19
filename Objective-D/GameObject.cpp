#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// GameObject Ŭ������ ��� ��ü���� ��ӹ޴� �θ� Ŭ�����̴�.
// ��� ��ü�� �ݵ�� �� Ŭ�����κ��� ��ӹ޾ƾ� Scene�� ��ü�� ������Ʈ�ϰ� �������Ѵ�.

// ��ü�� ������ ���¸� �ʱ�ȭ �Ѵ�. ��� ��ü�� �� �Լ��� ������ ������ ���۵ȴ�.
// �⺻ RANDER_TYPE_3D�� Ÿ���� �����Ǿ��ִ�.
// RENDER_TYPE_3D_STATIC�Ǵ� RENDER_TYPE_2D_STATIC�� ��� ��� �ʱ�ȭ�� �������� �ʴ´�. 
void GameObject::InitRenderState(int RenderTypeFlag) {
	// ��� ��� ����
	RenderType = RenderTypeFlag;

	if (RenderTypeFlag != RENDER_TYPE_3D_STATIC && RenderTypeFlag != RENDER_TYPE_2D_STATIC) {
		Transform::Identity(TranslateMatrix);
		Transform::Identity(RotateMatrix);
		Transform::Identity(ScaleMatrix);
	}

	if (RenderTypeFlag == RENDER_TYPE_2D || RenderTypeFlag == RENDER_TYPE_2D_STATIC) {
		Transform::Identity(ImageAspectMatrix);
		FlipTexture(FLIP_TYPE_NONE);
	}

	if (RenderTypeFlag == RENDER_TYPE_3D || RenderTypeFlag == RENDER_TYPE_3D_STATIC || RenderTypeFlag == RENDER_TYPE_3D_ORTHO) {
		// �ɼǿ� ���� �Ȱ��� �켱 ��Ȱ��ȭ �ǰų� �켱 Ȱ��ȭ �ȴ�.
		if (ENABLE_FOG_AFFTER_INIT_RENDER_STATE)
			SetFogUse(ENABLE_FOG);
		else
			SetFogUse(DISABLE_FOG);

		// �ɼǿ� ���� �ؽ�ó ���� ������ ����ǰų� ������� �ʴ´�.
		if (ENABLE_TEXTURE_V_FLIP_AFTER_INIT_RENDER_STATE)
			FlipTexture(FLIP_TYPE_V);
		else
			FlipTexture(FLIP_TYPE_NONE);
	}

	// �Ž� ���� �ʱ�ȭ
	SetColor(XMFLOAT3(0.0, 0.0, 0.0));

	// �ؽ�ó ���� �ʱ�ȭ
	ObjectAlpha = 1.0f;

	// ��� ��� ����
	switch(RenderTypeFlag) {
	case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		camera.SetToStaticMode();
		break;

	case RENDER_TYPE_3D: case RENDER_TYPE_3D_STATIC:
		SetLightUse(ENABLE_LIGHT);
		camera.SetToDefaultMode();
		break;
	}
}

// ��ü �޽��� ������ �����Ѵ�.
void GameObject::SetColor(XMFLOAT3& Color) {
	ObjectColor = Color;
}

// ��ü �޽��� ������ �����Ѵ�.
void GameObject::SetColor(float R, float G, float B) {
	ObjectColor.x = R;
	ObjectColor.y = G;
	ObjectColor.z = B;
}

// RGB���� ����Ͽ� ��ü �Ž��� ������ �����Ѵ�.
void GameObject::SetColorRGB(float R, float G, float B) {
	ObjectColor.x = 1.0 / 255.0 * float(R);
	ObjectColor.y = 1.0 / 255.0 * float(G);
	ObjectColor.z = 1.0 / 255.0 * float(B);
}

// �ؽ�ó�� ������Ų��. �𵨿� ���� �ٸ��� ����� �� �ִ�.
void GameObject::FlipTexture(int FlipType) {
	// �̹��� ��� ��忡���� ���� ������ �⺻ �����̱� ������ ������ �ٸ��� �����Ѵ�.
	if (RenderType == RENDER_TYPE_2D || RenderType == RENDER_TYPE_2D_STATIC) {
		switch (FlipType) {
		case FLIP_TYPE_V:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_NONE); break;

		case FLIP_TYPE_H:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_HV);   break;

		case FLIP_TYPE_HV:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_H);    break;

		case FLIP_TYPE_NONE:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_V);    break;
		}
	}

	else
		CBVUtil::Input(ObjectCmdList, FlipCBV, FlipType);
}

// ���� Ȱ��ȭ / ��Ȱ��ȭ
void GameObject::SetLightUse(int Flag) {
	CBVUtil::Input(ObjectCmdList, BoolLightCBV, Flag);
}

// �Ȱ� Ȱ��ȭ / ��Ȱ��ȭ
void GameObject::SetFogUse(int Flag) {
	CBVUtil::Input(ObjectCmdList, BoolFogCBV, Flag);
}

// 3D ������
void GameObject::Render3D(Mesh* MeshPtr, Texture* TexturePtr, float AlphaValue, bool DepthTestFlag) {
	TexturePtr->Render(ObjectCmdList);

	if(DepthTestFlag)
		ObjectShader->RenderDefault(ObjectCmdList);
	else
		ObjectShader->RenderDepthNone(ObjectCmdList);

	ObjectAlpha = AlphaValue;
	CBVUtil::Input(ObjectCmdList, LightCBV);
	CBVUtil::Input(ObjectCmdList, FogCBV);

	PrepareRender();
	MeshPtr->Render(ObjectCmdList);
}

// 2D ������
void GameObject::Render2D(Texture* TexturePtr, float AlphaValue, bool EnableAspect) {
	if(EnableAspect)
		Transform::ImageAspect(ImageAspectMatrix, TexturePtr->Width, TexturePtr->Height);
	TexturePtr->Render(ObjectCmdList);
	ImageShader->RenderDepthNone(ObjectCmdList);
	ObjectAlpha = AlphaValue;

	PrepareRender();
	ImagePannel->Render(ObjectCmdList);
}

// ���콺 ������κ��� ȸ���� ������Ʈ �Ѵ�.
void GameObject::UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY) {
	RotationX += DeltaY;
	RotationY += DeltaX;
}

// ���콺 ������κ��� ȸ���� ������Ʈ �Ѵ�.
void GameObject::UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY) {
	Rotation.x += DeltaY;
	Rotation.y += DeltaX;
}

// ��ŷ�� ���� ����� ������Ʈ �Ѵ�.
void GameObject::UpdatePickMatrix() {
	PickMatrix = ResultMatrix;
}

// ���� ��Ⱦ�� ���Ѵ�. UI�� �����Ҷ� �ַ� ����Ѵ�.
float GameObject::ASP(float Value) {
	return ASPECT * Value;
}

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& PickPosition, XMMATRIX& ViewMatrix, float* HitDistance) {
	int InterSected{};

	XMVECTOR PickRayOrigin, PickRayDirecton;
	GenPickingRay(PickPosition, ViewMatrix, PickRayOrigin, PickRayDirecton);
	InterSected = MeshPtr->CheckRayIntersection(PickRayOrigin, PickRayDirecton, HitDistance);

	return(InterSected);
}

//////////////////////////////////////// private

// ��İ� ���̴� �� ���� ���� ������ ���̴��� �����Ѵ�. Render�Լ��� �����ϸ� �� �Լ��� ����ȴ�. ��, ���� ����� ���� ����.
void GameObject::PrepareRender() {
	SetCamera();

	ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RotateMatrix), XMLoadFloat4x4(&TranslateMatrix));
	ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), ResultMatrix);

	// �̹��� ��� ����ϰ�� ��Ⱦ�� �����Ѵ�.
	if (RenderType == RENDER_TYPE_2D || RenderType == RENDER_TYPE_2D_STATIC)
		ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ImageAspectMatrix), ResultMatrix);

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(ObjectCmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(ObjectCmdList, &ObjectColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(ObjectCmdList, &ObjectAlpha, GAME_OBJECT_INDEX, 1, 19);
}

// ������ �� ī�޶� �����Ѵ�.
void GameObject::SetCamera() {
	// ī�޶� �� ����� �����Ѵ�.
	camera.SetViewMatrix();

	// ���� Ÿ�Կ� ���� �ٸ� ����� �ʱ�ȭ �Ѵ�.
	switch (RenderType) {
	case RENDER_TYPE_3D: case RENDER_TYPE_3D_STATIC:
		camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f); 
		break;

	case RENDER_TYPE_3D_ORTHO:
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0f, 10.0f);
		break;

	case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		camera.GenerateStaticMatrix();
	}

	camera.SetViewportsAndScissorRects();
	camera.UpdateShaderVariables();
}

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
void GameObject::GenPickingRay(XMVECTOR& PickPosition, XMMATRIX& ViewMatrix, XMVECTOR& PickRayOrigin, XMVECTOR& PickRayDirection) {
	XMMATRIX MatrixTomodel = XMMatrixInverse(NULL, PickMatrix * ViewMatrix);
	XMFLOAT3 CameraOrigin(0.0f, 0.0f, 0.0f);
	PickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&CameraOrigin), MatrixTomodel);
	PickRayDirection = XMVector3TransformCoord(PickPosition, MatrixTomodel);
	PickRayDirection = XMVector3Normalize(PickRayDirection - PickRayOrigin);
}