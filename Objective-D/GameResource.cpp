#include "GameResource.h"
#include <random>
// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Scene::Init()���� ����ȴ�.


Mesh* GunMesh;

// �Ž��� ���⼭ �ε��Ѵ�.
void LoadMesh(DeviceSystem& System) {
	ImportMesh(System, GunMesh, "Resources//Models//model.bin", MESH_TYPE_BIN);
}
/////////////////////////////////////////////////////////////////////////////////


Texture* Tex, * SkyboxTex, * WoodTex;

// �ý�ó�� ���⼭ �ε��Ѵ�.
void LoadTexture(DeviceSystem& System) {
	ImportTexture(System, Tex, L"Resources//Image//Gun.jpg", TEXTURE_TYPE_WIC);
	ImportTexture(System, WoodTex, L"Resources//Image//Wood.jpg", TEXTURE_TYPE_WIC);
}
/////////////////////////////////////////////////////////////////////////////////


Object_Shader* ObjectShader;
Boundbox_Shader* BoundboxShader;
Image_Shader* ImageShader;
Line_Shader* LineShader;

// ���̴��� ���⼭ �ε��Ѵ�.
void LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device) {
	// �Ϲ� ������ ���̴� ����
	ObjectShader = new Object_Shader();
	// �⺻ ���������� ����
	ObjectShader->CreateDefaultPS(Device, RootSignature);
	// ���� �˻� ������ ���������� ����
	ObjectShader->CreateNoneDepthPS(Device, RootSignature);

	// �̹��� ��¿� ���������� ����
	ImageShader = new Image_Shader();
	ImageShader->CreateNoneDepthPS(Device, RootSignature);

	// �ٿ��ڽ� ���̴� ����
	BoundboxShader = new Boundbox_Shader();
	BoundboxShader->CreateWireframePS(Device, RootSignature);
	BoundboxShader->CreateDefaultPS(Device, RootSignature);

	// ���� �귯�� ��¿� ���̴� ����
	LineShader = new Line_Shader();
	LineShader->CreateNoneDepthPS(Device, RootSignature);
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// ���ε� ���۸� ó���ϱ� ���� ����
std::vector<Mesh*> LoadedMeshList;
std::vector<Texture*> LoadedTextureList;

Mesh* ImagePannel;
Mesh* SkyboxMesh;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;

// �⺻ ���� �Ž� �ε�
void LoadSystemMesh(DeviceSystem& System) {
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(System.Device, System.CmdList);
	LoadedMeshList.emplace_back(ImagePannel);

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(System.Device, System.CmdList);
	LoadedMeshList.emplace_back(SkyboxMesh);

	// �ٿ��ڽ� ��¿� �Ž� ����
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(System.Device, System.CmdList);
	LoadedMeshList.emplace_back(BoundMesh);

	// �ٿ�彺��� ��¿� �Ž� ����
	BoundingSphereMesh = new Mesh(System.Device, System.CmdList, "Resources//SystemResources//Models//BoundingSphereMesh.txt", MESH_TYPE_TEXT);
	LoadedMeshList.emplace_back(BoundingSphereMesh);
}

// ���ε� ���۸� �����ϰ�, ���͸� ����.
void ClearUploadBuffer() {
	for (auto const& MeshPtr : LoadedMeshList)
		MeshPtr->ReleaseUploadBuffers();

	for (auto const& TexturePtr : LoadedTextureList)
		TexturePtr->ReleaseUploadBuffers();

	LoadedMeshList.clear();
	LoadedTextureList.clear();
}