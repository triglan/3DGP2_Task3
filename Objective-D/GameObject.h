#pragma once
#include "GameResource.h"
#include "CBVUtil.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"
#include "LineBrush.h"
#include "TerrainUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);

class GameObject {
public:
	// �� ��ü�� ������ ����� ������.
	XMFLOAT4X4 TranslateMatrix{ Mat4::Identity() };
	XMFLOAT4X4 RotateMatrix{ Mat4::Identity() };
	XMFLOAT4X4 ScaleMatrix { Mat4::Identity() };
	XMFLOAT4X4 ImageAspectMatrix{ Mat4::Identity() };
	XMMATRIX ResultMatrix{};

	// ������Ʈ ��ŷ �� ��� ���� ���
	XMMATRIX PickMatrix{};

	// �Ž� ����
	XMFLOAT3 ObjectColor{};

	// �ؽ�ó ����
	float ObjectAlpha{ 1.0f };

	// ������ Ÿ��, �ش� ������ Ÿ�Կ� ���� ������ ������ �޶�����.
	int RenderType{ RENDER_TYPE_3D };

	int ObjectLayer{};
	std::string ObjectTag{};
	bool DeleteCommand{};
	bool DeleteReserveCommand{};

	void InitRenderState(int RenderTypeFlag = RENDER_TYPE_3D);
	void SetColor(XMFLOAT3& Color);
	void SetColor(float R, float G, float B);
	void SetColorRGB(float R, float G, float B);
	void SetLightUse(int Flag);
	void SetFogUse(int Flag);
	void FlipTexture(int FlipType);
	float ASP(float Value);
	void Render3D(Mesh* MeshPtr, Texture* TexturePtr, float AlphaValue=1.0f, bool DepthTestFlag=true);
	void Render2D(Texture* TexturePtr, float AlphaValue=1.0f, bool EnableAspect=true);
	void UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY);
	void UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY);
	void UpdatePickMatrix();
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& PickPosition, XMMATRIX& ViewMatrix, float* HitDistance);

private:
	void PrepareRender();
	void SetCamera();
	void GenPickingRay(XMVECTOR& PickPosition, XMMATRIX& ViewMatrix, XMVECTOR& PickRayOrigin, XMVECTOR& PickRayDirection);

	////////// virtual functions
public:
	// �Ʒ� �Լ����� ��� ��ü���� Ŀ���� ������ ���߾� �Լ����̴�. �ʿ��ϴٸ� ���ο� ���߾� �Լ��� �ۼ��Ͽ� ����� �� �ִ�.
	virtual ~GameObject() {}
	virtual void ReleaseShaderVariables() {}
	virtual void InputKey(KeyEvent& Event) {}
	virtual void InputMouse(MouseEvent& Event) {}
	virtual void InputMouseMotion(MotionEvent& Event) {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual Mesh* GetObjectMesh() { return {}; }
	virtual XMFLOAT3 GetPosition() { return {}; }
	virtual OOBB GetAABB() { return {}; }
	virtual OOBB GetOOBB() { return {}; }
	//virtual OOBB GetRange() { return {}; }
	virtual Range GetRange() { return {}; }
	virtual Vector GetVectorSet() { return {}; }
	virtual TerrainUtil GetTerrain() { return {}; }

	// ����� ���� ���� �Լ��� �Ʒ��� �����Ѵ�.
};