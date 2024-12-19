#include "GameResource.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Scene::Init()���� ����ȴ�.

////////////////////////////////
BasicObjectShader* ObjectShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex, * GuideTex;
Texture* TreeTex; // ���� ������ �ؽ�ó
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
////////////////////////////////

Mesh* HelicopterBodyMesh, * HelicopterHeadMesh;
Mesh* TerrainMesh;
Texture* HelicopterTex, * WoodTex, * SkyboxTex, * TerrainTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// ���������� ������ �� ���̴� ������ �������̴�.
	// �Ϲ� ������ ���̴� ����
	ObjectShader = new BasicObjectShader();
	ObjectShader->CreateDefaultPipeline(Device, RootSignature);

	// ���� �˻� ������ ���������� ����
	ObjectShader->CreateImageDepthPipelineState(Device, RootSignature);

	// �ٿ��ڽ� ���̴� ����
	BoundboxShader = new BasicObjectShader();
	BoundboxShader->CreateBoundboxPipeline(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �̹��� ��¿� �г� ����
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	// �ٿ��ڽ� ��¿� �Ž� ����
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);

	// �ٿ�彺��� ��¿� �Ž� ����
	BoundingSphereMesh = new Mesh(Device, CmdList, "Resources//SystemResources//Models//BoundingSphereMesh.txt", MESH_TYPE_TEXT);
	////////////////////////////////

	// �︮����
	HelicopterBodyMesh = new Mesh(Device, CmdList, "Resources//Models//GunShip.bin", MESH_TYPE_BIN);
	HelicopterHeadMesh = new Mesh(Device, CmdList, "Resources//Models//Rotor.bin", MESH_TYPE_BIN);

	// ����
	TerrainMesh = new Mesh(Device, CmdList, "Resources//Models//terrain.bin", MESH_TYPE_BIN);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// guide
	LineTex = new Texture(Device, CmdList, L"Resources//SystemResources//Textures//line_tex.png", TEXTURE_TYPE_WIC);
	GuideTex = new Texture(Device, CmdList, L"Resources//Image//guide.png", TEXTURE_TYPE_WIC);
	////////////////////////////////

	TreeTex = new Texture(Device, CmdList, L"Resources//Image//tree.png", TEXTURE_TYPE_WIC);
	TerrainTex = new Texture(Device, CmdList, L"Resources//Image//grass.jpg", TEXTURE_TYPE_WIC);
	HelicopterTex = new Texture(Device, CmdList, L"Resources//Image//GunShip.png", TEXTURE_TYPE_WIC);
	WoodTex = new Texture(Device, CmdList, L"Resources//Image//Wood.jpg", TEXTURE_TYPE_WIC);
	SkyboxTex = new Texture(Device, CmdList, L"Resources//Image//skytex.png", TEXTURE_TYPE_WIC);
}