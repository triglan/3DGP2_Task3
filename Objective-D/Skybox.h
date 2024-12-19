#pragma once
#include "GameObject.h"
#include "CameraUtil.h"

class Skybox : public GameObject {
public:
	Skybox() {
		Transform::Scale(ScaleMatrix, 400.0, 400.0, 400.0);

	}

	void Render(CommandList CmdList) {
		RenderType = RENDER_TYPE_PERS;
		DisableLight(CmdList);
		ObjectAlpha = 1.0f;
		camera.SetToDefaultMode();
		SetColor(0.0, 0.0, 0.0);
		FlipTexture(CmdList, FLIP_TYPE_NONE);

		RenderMesh(CmdList, SkyboxMesh, SkyboxTex, ObjectShader);
	}
};