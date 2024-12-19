#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "GameResource.h"
#include "TransformUtil.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
public:
	Terrain(int x, int z) {
		// 터레인 정보를 터레인 충돌처리 유틸에 전달한다.
		Transform::Move(TranslateMatrix, x, -50.0, z);
		Transform::Scale(ScaleMatrix, 30.0, 30.0, 30.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}

	void Render(CommandList CmdList) {
		// 터레인은 정적 오브젝트이므로 행렬 초기화 및 변환을 실행하지 않는다.
		//EnableLight(CmdList);
		//FlipTexture(CmdList, false);
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);

		Render3D(TerrainMesh, TerrainTex);

		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, TerrainMesh);
	}
};
