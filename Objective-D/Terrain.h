#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "GameResource.h"
#include "TransformUtil.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
public:
	Terrain(int x, int z) {
		// �ͷ��� ������ �ͷ��� �浹ó�� ��ƿ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, x, -50.0, z);
		Transform::Scale(ScaleMatrix, 30.0, 30.0, 30.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}

	void Render() override {
		// �ͷ����� ���� ������Ʈ�̹Ƿ� ��� �ʱ�ȭ �� ��ȯ�� �������� �ʴ´�.
		//EnableLight(CmdList);
		//FlipTexture(CmdList, false);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		Render3D(TerrainMesh, TerrainTex);

		//terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}
};