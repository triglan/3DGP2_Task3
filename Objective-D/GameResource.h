#pragma once
#include "MeshUtil.h"
#include "TextureUtil.h"
#include "ImageShader.h"
#include "ObjectShader.h"
#include "BoundboxShader.h"
#include "LineShader.h"

// �Ž�, �ؽ�ó, ���̴� ���� ������ ���⿡ extern ����
extern Mesh* GunMesh;
extern Mesh* HelicopterBodyMesh, * HelicopterHeadMesh;
extern Mesh* TerrainMesh;

/////////////////////////////////////////////////////////////////////////////////

extern Texture* Tex, * SkyboxTex, * WoodTex;
extern Texture * GuideTex, * TreeTex;
extern Texture* HelicopterTex, * TerrainTex;

/////////////////////////////////////////////////////////////////////////////////

extern Object_Shader* ObjectShader;
extern Boundbox_Shader* BoundboxShader;
extern Image_Shader* ImageShader;
extern Line_Shader* LineShader;
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////// �⺻ ���ҽ�
// �Ž�, �ؽ�ó �ε� �� �����͸� �����ϴ� ����
// �Ѳ����� ���ε� ���۸� ������
extern std::vector<Mesh*> LoadedMeshList;
extern std::vector<Texture*> LoadedTextureList;

extern Mesh* ImagePannel;
extern Mesh* SkyboxMesh;
extern Mesh* BoundMesh;
extern Mesh* BoundingSphereMesh;

void LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device);
void LoadSystemMesh(DeviceSystem& System);
void LoadMesh(DeviceSystem& System);
void LoadTexture(DeviceSystem& System);
void ClearUploadBuffer();

inline void ImportMesh(DeviceSystem& System, Mesh*& MeshPtr, char* Directory, int Type) {
	MeshPtr = new Mesh(System.Device, System.CmdList, Directory, Type);
	LoadedMeshList.emplace_back(MeshPtr);
}

inline void ImportTexture(DeviceSystem& System, Texture*& TexturePtr, wchar_t* Directory, int Type, D3D12_FILTER FilterOption=D3D12_FILTER_MIN_MAG_MIP_LINEAR) {
	TexturePtr = new Texture(System.Device, System.CmdList, Directory, Type, FilterOption);
	LoadedTextureList.emplace_back(TexturePtr);
}