#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "TerrainUtil.h"

// 나무 빌보드

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

	void Render() override {
		InitRenderState(RENDER_TYPE_3D);

		// 조명 비활성화
		SetLightUse(DISABLE_LIGHT);

		Transform::Move(TranslateMatrix, Position.x, Position.y - 40, Position.z);
		Transform::Scale(ScaleMatrix, 10.0, 10.0, 1.0);


		// 카메라를 바라보도록 한다.
		Math::BillboardLookAt(RotateMatrix, Vec, Position, camera.GetPosition());

		FlipTexture(FLIP_TYPE_V);
		Render3D(ImagePannel, TreeTex);
	}
};