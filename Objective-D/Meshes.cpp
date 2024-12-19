#include "MeshUtil.h"

// ���⿡ �Ž��� �ۼ��Ѵ�.

// ��ī�̹ڽ� �Ž��� �����Ѵ�. ���α׷� ���� �� ���� 1ȸ ����ǰ� �� ���Ŀ��� ���� ������ �ʿ� ����.
void Mesh::CreateSkyboxMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	float SkyboxVertices[][8] = {
		// x, y, z, nx, ny, nz, u, v
	   // Front face (�߾�)
	   { -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.2509f, 0.3339f },  // Top-left
	   {  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.4991f, 0.3339f },  // Top-right
	   {  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.4991f, 0.6661f },  // Bottom-right
	   { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.2509f, 0.6661f },  // Bottom-left

	   // Back face (������)
	   {  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.7491f, 0.3339f },  // Top-left
	   { -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.9991f, 0.3339f },  // Top-right
	   { -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.9991f, 0.6661f },  // Bottom-right
	   {  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.7491f, 0.6661f },  // Bottom-left

	   // Left face (����)
	   { -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0009f, 0.3339f },   // Top-left
	   { -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.2491f, 0.3339f },   // Top-right
	   { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.2491f, 0.6661f },   // Bottom-right
	   { -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0009f, 0.6661f },   // Bottom-left

	   // Right face (������ �߾�)
	   {  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.5009f, 0.3339f },    // Top-left
	   {  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.7491f, 0.3339f },    // Top-right
	   {  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.7491f, 0.6661f },    // Bottom-right
	   {  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.5009f, 0.6661f },    // Bottom-left

	   // Top face (�߾� �Ʒ�)
	   { -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.2509f, 0.0009f },    // Top-left
	   {  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.4991f, 0.0009f },    // Top-right
	   {  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.4991f, 0.3339f },    // Bottom-right
	   { -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.2509f, 0.3339f },    // Bottom-left

	   // Bottom face (�߾� ��)
	   { -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.2509f, 0.6661f },   // Top-left
	   {  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.4991f, 0.6661f },   // Top-right
	   {  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.4991f, 0.9991f },   // Bottom-right
	   { -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.2509f, 0.9991f }    // Bottom-left
	};

	// �ε��� �迭 (�� ���� �� ���� �ﰢ������ ����)
	UINT indices[] = {
		// Front face (�ð� ����)
		0, 2, 1, 0, 3, 2,
		// Back face (�ð� ����)
		4, 6, 5, 4, 7, 6,
		// Left face (�ð� ����)
		8, 10, 9, 8, 11, 10,
		// Right face (�ð� ����)
		12, 14, 13, 12, 15, 14,
		// Top face (�ð� ����)
		16, 18, 17, 16, 19, 18,
		// Bottom face (�ð� ����)
		20, 22, 21, 20, 23, 22
	};


	// ���� ������ �ε��� ���� ����
	Vertices = 24;
	Indices = 36;

	// ���� �� ���� ������ �Ҵ�
	Position = new XMFLOAT3[Vertices];
	Normal = new XMFLOAT3[Vertices];
	TextureCoords = new XMFLOAT2[Vertices];
	PnIndices = new UINT[Indices];

	// SkyboxVertices �����͸� �̿��Ͽ� �� ������ ������, �븻, �ؽ�ó ��ǥ�� ����
	for (UINT i = 0; i < Vertices; i++) {
		Position[i] = XMFLOAT3(SkyboxVertices[i][0], SkyboxVertices[i][1], SkyboxVertices[i][2]);
		Normal[i] = XMFLOAT3(SkyboxVertices[i][3], SkyboxVertices[i][4], SkyboxVertices[i][5]);
		TextureCoords[i] = XMFLOAT2(SkyboxVertices[i][6], SkyboxVertices[i][7]);
	}

	// �ε��� ������ ����
	for (UINT i = 0; i < Indices; i++)
		PnIndices[i] = indices[i];

	// DirectX ���� ���� �� �� ���� (���� �ڵ�� ����)
	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);
	NormalBuffer = ::CreateBufferResource(Device, CmdList, Normal, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &NormalUploadBuffer);
	TextureCoordBuffer = ::CreateBufferResource(Device, CmdList, TextureCoords, sizeof(XMFLOAT2) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &TextureCoordUploadBuffer);

	// ���� �� ����
	NumVertexBufferViews = 3;
	VertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[NumVertexBufferViews];

	VertexBufferViews[0].BufferLocation = PositionBuffer->GetGPUVirtualAddress();
	VertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[1].BufferLocation = NormalBuffer->GetGPUVirtualAddress();
	VertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[2].BufferLocation = TextureCoordBuffer->GetGPUVirtualAddress();
	VertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	VertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * Vertices;

	// �ε��� ���� ����
	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	// �ε��� ���� �� ����
	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
}

// �̹��� ����� ���� �β��� ���� ���簢�� �Ž��� �����Ѵ�. ���α׷� ���� �� ���� 1ȸ ����ǰ� �� ���Ŀ��� ���� ������ �ʿ� ����.
void Mesh::CreateImagePannelMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	float ImagePannel[][8] = {
		// x, y, z, nx, ny, nz, u, v
		{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }
	};

	// �ε��� �迭
	UINT indices[] = { 0, 2, 1, 0, 3, 2 };

	// ���� ������ �ε��� ����
	Vertices = 4;
	Indices = 6;

	// ���� �� ���� ������ �Ҵ�
	Position = new XMFLOAT3[Vertices];
	Normal = new XMFLOAT3[Vertices];
	TextureCoords = new XMFLOAT2[Vertices];
	PnIndices = new UINT[Indices];

	// ImagePannel �����͸� �̿��Ͽ� �� ������ ������, �븻, �ؽ�ó ��ǥ�� ����
	for (UINT i = 0; i < Vertices; i++) {
		Position[i] = XMFLOAT3(ImagePannel[i][0], ImagePannel[i][1], ImagePannel[i][2]);
		Normal[i] = XMFLOAT3(ImagePannel[i][3], ImagePannel[i][4], ImagePannel[i][5]);
		TextureCoords[i] = XMFLOAT2(ImagePannel[i][6], ImagePannel[i][7]);
	}

	// �ε��� ������ ����
	for (UINT i = 0; i < Indices; i++) {
		PnIndices[i] = indices[i];
	}

	// DirectX ���� ���� (�� �ڵ��� ImportMesh �Լ��� ����)
	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);
	NormalBuffer = ::CreateBufferResource(Device, CmdList, Normal, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &NormalUploadBuffer);
	TextureCoordBuffer = ::CreateBufferResource(Device, CmdList, TextureCoords, sizeof(XMFLOAT2) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &TextureCoordUploadBuffer);

	// ���� �� ����
	NumVertexBufferViews = 3;
	VertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[NumVertexBufferViews];

	VertexBufferViews[0].BufferLocation = PositionBuffer->GetGPUVirtualAddress();
	VertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[1].BufferLocation = NormalBuffer->GetGPUVirtualAddress();
	VertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[2].BufferLocation = TextureCoordBuffer->GetGPUVirtualAddress();
	VertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	VertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * Vertices;

	// �ε��� ���� ����
	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	// �ε��� ���� �� ����
	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
}

// �ٿ�� �ڽ� ����� ���� �Ž��� �����Ѵ�. ���α׷� ���� �� ���� 1ȸ ����ǰ� �� ���Ŀ��� ���� ������ �ʿ� ����.
void Mesh::CreateBoundboxMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	float BoundboxVertices[][8] = {
		// x, y, z, nx, ny, nz, u, v
		// Front face (�߾�)
		{ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },  // Top-left
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },  // Bottom-left
		{  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },  // Bottom-right
		{  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },  // Top-right

		// Back face (������)
		{  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f },  // Top-left
		{  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f },  // Bottom-left
		{ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f },  // Bottom-right
		{ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f },  // Top-right

		// Left face (����)
		{ -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },   // Top-left
		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },   // Bottom-left
		{ -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },   // Bottom-right
		{ -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },   // Top-right

		// Right face (������ �߾�)
		{  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },    // Top-left
		{  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },    // Bottom-left
		{  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },    // Bottom-right
		{  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },    // Top-right

		// Top face (�߾� �Ʒ�)
		{ -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },    // Top-left
		{ -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },    // Bottom-left
		{  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },    // Bottom-right
		{  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },    // Top-right

		// Bottom face (�߾� ��)
		{ -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },   // Top-left
		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },   // Bottom-left
		{  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },   // Bottom-right
		{  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f }    // Top-right
	};

	UINT indices[] = {
		// Front face (�ݽð� ����)
		0, 1, 2, 0, 2, 3,
		// Back face (�ݽð� ����)
		4, 5, 6, 4, 6, 7,
		// Left face (�ݽð� ����)
		8, 9, 10, 8, 10, 11,
		// Right face (�ݽð� ����)
		12, 13, 14, 12, 14, 15,
		// Top face (�ݽð� ����)
		16, 17, 18, 16, 18, 19,
		// Bottom face (�ݽð� ����)
		20, 21, 22, 20, 22, 23
	};

	// ���� ������ �ε��� ���� ����
	Vertices = 24;
	Indices = 36;

	// ���� �� ���� ������ �Ҵ�
	Position = new XMFLOAT3[Vertices];
	PnIndices = new UINT[Indices];

	// �� ������ ������ ��ǥ�� ����
	for (UINT i = 0; i < Vertices; i++) 
		Position[i] = XMFLOAT3(BoundboxVertices[i][0], BoundboxVertices[i][1], BoundboxVertices[i][2]);
	
	// �ε��� ������ ����
	for (UINT i = 0; i < Indices; i++) 
		PnIndices[i] = indices[i];
	
	// DirectX ���� ���� �� �� ���� (���� �ڵ�� ����)
	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);

	// ���� �� ����
	NumVertexBufferViews = 1;
	VertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[NumVertexBufferViews];

	VertexBufferViews[0].BufferLocation = PositionBuffer->GetGPUVirtualAddress();
	VertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	// �ε��� ���� ����
	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	// �ε��� ���� �� ����
	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
}

// ���Ϸκ��� �Ž��� �ҷ��´�,
void Mesh::ImportMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode) {
	char Token[64] = { '\0' };

	if (TextMode) {
		std::ifstream In(Directory);

		while (true) {
			In >> Token;
			if (!In) break;

			if (!strcmp(Token, "<Vertices>:")) {
				In >> Vertices;
				Position = new XMFLOAT3[Vertices];
				for (UINT i = 0; i < Vertices; i++) In >> Position[i].x >> Position[i].y >> Position[i].z;
			}

			else if (!strcmp(Token, "<Normals>:")) {
				In >> Token;
				Normal = new XMFLOAT3[Vertices];
				for (UINT i = 0; i < Vertices; i++) In >> Normal[i].x >> Normal[i].y >> Normal[i].z;
			}

			else if (!strcmp(Token, "<TextureCoords>:")) {
				In >> Token;
				TextureCoords = new XMFLOAT2[Vertices];
				for (UINT i = 0; i < Vertices; i++) In >> TextureCoords[i].x >> TextureCoords[i].y;
			}

			else if (!strcmp(Token, "<Indices>:")) {
				In >> Indices;
				PnIndices = new UINT[Indices];
				for (UINT i = 0; i < Indices; i++) In >> PnIndices[i];
			}
		}
	}

	else {
		FILE* File = NULL;
		::fopen_s(&File, Directory, "rb");
		::rewind(File);

		char Token[64] = { '\0' };

		BYTE StrLength = 0;
		UINT Reads = 0;

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 14, File); //"<BoundingBox>:"
		Reads = (UINT)::fread(&OOBB.Center, sizeof(float), 3, File);
		Reads = (UINT)::fread(&OOBB.Extents, sizeof(float), 3, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 11, File); //"<Vertices>:"
		Reads = (UINT)::fread(&Vertices, sizeof(int), 1, File);
		Position = new XMFLOAT3[Vertices];
		Reads = (UINT)::fread(Position, sizeof(float), 3 * Vertices, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 10, File); //"<Normals>:"
		Reads = (UINT)::fread(&Vertices, sizeof(int), 1, File);
		Normal = new XMFLOAT3[Vertices];
		Reads = (UINT)::fread(Normal, sizeof(float), 3 * Vertices, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 16, File); //"<TextureCoords>:"
		Reads = (UINT)::fread(&Vertices, sizeof(int), 1, File);
		TextureCoords = new XMFLOAT2[Vertices];
		Reads = (UINT)::fread(TextureCoords, sizeof(float), 2 * Vertices, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 10, File); //"<Indices>:"
		Reads = (UINT)::fread(&Indices, sizeof(int), 1, File);
		PnIndices = new UINT[Indices];
		Reads = (UINT)::fread(PnIndices, sizeof(UINT), Indices, File);

		::fclose(File);
	}

	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);
	NormalBuffer = ::CreateBufferResource(Device, CmdList, Normal, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &NormalUploadBuffer);
	TextureCoordBuffer = ::CreateBufferResource(Device, CmdList, TextureCoords, sizeof(XMFLOAT2) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &TextureCoordUploadBuffer);

	NumVertexBufferViews = 3;
	VertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[NumVertexBufferViews];

	VertexBufferViews[0].BufferLocation = PositionBuffer->GetGPUVirtualAddress();
	VertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[1].BufferLocation = NormalBuffer->GetGPUVirtualAddress();
	VertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[2].BufferLocation = TextureCoordBuffer->GetGPUVirtualAddress();
	VertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	VertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * Vertices;

	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
}