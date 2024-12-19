#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// �ͷ��� �浹ó�� ��ƿ�̴�.

// Ư�� ������Ʈ�� �ͷ����� �ٴڿ� ��Ҵ��� �˻��Ѵ�.
// ����� ��� true�� �����Ѵ�.
bool TerrainUtil::CheckFloor(XMFLOAT3 Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix))
			return true;
	}

	return false;
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴ��� ��ġ�� �̵���Ų��.
// �� �Լ��� ������Ʈ �������� ����� ��� ������Ʈ�� �ͷ��� �ٴڿ��� �������� �ʰ� �ȴ�.
void TerrainUtil::ClampToFloor(XMFLOAT3& Position, float Offset) {
	if (TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + Offset;
}

float TerrainUtil::GetFloorHeight(float x, float z, float Offset) {
	if (TerrainMesh)
		return TerrainMesh->GetHeightAtPosition(TerrainMesh, x, z, TerrainMatrix) + Offset;
	return 0.0;
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴڿ� ������ ��� �ͷ��� �ٴ��� ���̷� ������Ʈ�� �̵���Ų��
void TerrainUtil::CheckCollision(XMFLOAT3& Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix))
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix);
	}
}

void TerrainUtil::InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMat), XMLoadFloat4x4(&RMat));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMat));
	XMStoreFloat4x4(&TerrainMatrix, XMMatrixTranspose(ResultMatrix));

	TerrainMesh = MeshData;
}