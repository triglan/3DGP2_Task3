#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>
#include "PickingUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{0.0, 0.0, 0.0};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	XMFLOAT3 Color{ 0.0, 0.0, 0.0 };

	LineBrush line;

	OOBB oobb;
	
	float point = 0.1;

	TestObject() {
		line.SetColor(1.0, 1.0, 1.0);
		camera.Move(XMFLOAT3(0.0, 0.0, -2.0));
		Position.z = 5.0;
	}

	void InputMouseMotion(MotionEvent& Event) {
		if (GetCapture() == Event.CaptureState) {
			mouse.HideCursor();
			GetCapture();

			XMFLOAT2 Delta = mouse.GetMotionDelta(Event.Motion, 0.5);
			UpdateMotionRotation(RotationDest, Delta.x, Delta.y);
		}
	}

	void InputMouse(MouseEvent& Event) {
		switch (Event.Type) {
		case WM_LBUTTONDOWN:
			mouse.StartMotionCapture(Event.hWnd);
			break;

		case WM_LBUTTONUP:
			mouse.EndMotionCapture();
			break;

		case WM_RBUTTONDOWN:
			if (PickingUtil::PickByViewportOOBB(mouse.x, mouse.y, oobb)) {
				Color = XMFLOAT3(1.0, 0.0, 0.0);
			}
			else
				Color = XMFLOAT3(0.0, 0.0, 0.0);
			break;
		}
	}

	void Update(float FT) {
		Rotation.x = std::lerp(Rotation.x, RotationDest.x, FT * 10);
		Rotation.y = std::lerp(Rotation.y, RotationDest.y, FT * 10);
	}

	void Render() override {
		/*InitRenderState(RENDER_TYPE_3D);
		SetLightUse(DISABLE_LIGHT);
		Transform::Scale(ScaleMatrix, 20.0, 20.0, 20.0);
		Render3D(SkyboxMesh, SkyboxTex, 1.0, false);*/

		// �� ���
		InitRenderState(RENDER_TYPE_3D);
		SetColor(Color);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y, 0.0);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Render3D(GunMesh, Tex);

		// ��ŷ ��� ������Ʈ, ������ ���� ������ ���� ��õ
		UpdatePickMatrix();

		oobb.Update(GunMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);
		oobb.Render();

		// �̹��� ���, �̹��� ��Ⱦ�� �ڵ����� ����ȴ�.
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move2D(TranslateMatrix, -0.5, 0.5);
		Transform::Scale2D(ScaleMatrix, 0.5, 0.5);
		Render2D(WoodTex);
	}
};