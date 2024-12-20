#pragma once
#include "GameObject.h"
#include "CameraUtil.h"

class Skybox : public GameObject {
public:
	Skybox() {
		

	}

	void Render() override {
		InitRenderState(RENDER_TYPE_3D);

		Transform::Scale(ScaleMatrix, 400.0, 400.0, 400.0);
		SetLightUse(DISABLE_LIGHT);
		ObjectAlpha = 1.0f;
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);
		FlipTexture(FLIP_TYPE_NONE);

		Render3D(SkyboxMesh, SkyboxTex);

	}
};