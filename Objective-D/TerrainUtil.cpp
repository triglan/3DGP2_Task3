#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// �ͷ��� �浹ó�� ��ƿ�̴�.

// �ͷ����� ������ �Է��Ѵ�.
// �� �ͷ��� �Ž� �� �� ���� �����ϸ� �ȴ�. �����ڿ��� ������ ���� �����Ѵ�.
// �ͷ����� �ƴ� ��ü�� �ش� �Լ��� ������ �ʿ䰡 ����.
void TerrainUtil::InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMat), XMLoadFloat4x4(&RMat));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMat));
	XMStoreFloat4x4(&TerrainMatrix, XMMatrixTranspose(ResultMatrix));

	TerrainMesh = MeshData;

	// ���� ĳ�ø� ���� ���ο� ������ ���� ĳ�ø� �����Ѵ�.
	TerrainMesh->ClearHeightCache();
	TerrainMesh->SetHeightCache(TerrainMesh, TerrainMatrix);
}

// �Ʒ��� �Լ����� �ͷ��� ��ü���� ���� �� �ʿ� ����.

// ���� ��ġ�� ���� �������� �Է��Ѵ�
void TerrainUtil::InputPosition(XMFLOAT3& PositionValue, float HeightOffsetValue) {
	Position = PositionValue;
	HeightOffset = HeightOffsetValue;
}

// �ͷ����� ���̷� ��ü ���� ��ġ�� �����Ѵ�.
void TerrainUtil::SetHeightToTerrain(XMFLOAT3& PositionValue) {
	PositionValue.y = Position.y;
}

// �Է��� ���̰� �ͷ����� �ٴ� ���̺��� ������ �˻��Ѵ�. ���� ��� true�� �����Ѵ�.
bool TerrainUtil::CheckCollision(const TerrainUtil& Other) {
	float Height = Other.TerrainMesh->GetHeightAtPosition(Other.TerrainMesh, Position.x, Position.z, Other.TerrainMatrix) + HeightOffset;

	if (Position.y < Height) {
		Position.y = Height;
		return true;
	}

	return false;
}