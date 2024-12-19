#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "TerrainUtil.h"

// ���� ������

class Tree : public GameObject {
private:
	Vector Vec{};
	XMFLOAT3 Position{};
	float Rotation{};

public:
	Tree(float X, float Z) {
		Math::InitVector(Vec);
		Position.x = X;
		Position.z = Z;

		Position.y = terrainUtil.GetFloorHeight(Position.x, Position.z, 0.0);
	}

	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RENDER_TYPE_PERS);

		// ���� ��Ȱ��ȭ
		DisableLight(CmdList);

		Transform::Move(TranslateMatrix, Position.x, Position.y - 40, Position.z);
		Transform::Scale(ScaleMatrix, 10.0, 10.0, 1.0);


		// ī�޶� �ٶ󺸵��� �Ѵ�.
		Math::BillboardLookAt(RotateMatrix, Vec, Position, camera.GetPosition());

		FlipTexture(CmdList, FLIP_TYPE_V);
		RenderMesh(CmdList, ImagePannel, TreeTex, ObjectShader);
	}
};