#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

////////////////////////////////// �⺻ ���ҽ�
extern BasicObjectShader* ObjectShader;
extern BasicObjectShader* BoundboxShader;

// �� ��¿� �ؽ�ó
extern Texture* LineTex;

// �̹��� ��¿� �Ž�
extern Mesh* ImagePannel;

// ��ī�̹ڽ� ��¿� �Ž�
extern Mesh* SkyboxMesh;

// �ٿ��ڽ� ��¿� �Ž�
extern Mesh* BoundMesh;

// �ٿ�� ����� ��¿� �Ž�
extern Mesh* BoundingSphereMesh;
//////////////////////////////////

// Ŀ���� �Ž� �� �ؽ�ó
extern Mesh* HelicopterBodyMesh, * HelicopterHeadMesh, * TerrainMesh;
extern Texture* HelicopterTex, * WoodTex, * SkyboxTex, * TerrainTex, * TreeTex, * GuideTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);