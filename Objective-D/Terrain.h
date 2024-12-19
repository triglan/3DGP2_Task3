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

	void Render(CommandList CmdList) {
		// �ͷ����� ���� ������Ʈ�̹Ƿ� ��� �ʱ�ȭ �� ��ȯ�� �������� �ʴ´�.
		EnableLight(CmdList);
		FlipTexture(CmdList, false);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		RenderMesh(CmdList, TerrainMesh, TerrainTex, ObjectShader);


		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);


	}
};
