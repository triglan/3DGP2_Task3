#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

#include "LineBrush.h"
#include "Mode0.h"
#include "Config.h"

class Control : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float WingRotation{ 90 };
	XMFLOAT3 HeliRotation{ 0, 0, 0};
	XMFLOAT3 Tilt{0, 0, 0};
	bool ColBack{};
	
public:
	XMFLOAT3 Position{0.0, 0.0, 0.0};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	float GunAlpha = 1.0;

	bool UseLight{};

	LineBrush line;

	OOBB oobb;
	Range range;
	//Range GetRange() { return range; }
	OOBB GetRANGE;//--

	Vector Vec{};

	Control() {
		Math::InitVector(Vec);
		line.SetColor(1.0, 1.0, 1.0);
	}

	XMFLOAT3 GetUp() {
		return Vec.Up;
	}

	XMFLOAT3 GetLook() {
		return Vec.Look;
	}

	XMFLOAT3 GetRight() {
		return Vec.Right;
	}

	XMFLOAT3 GetPosition() {
		return Position;
	}
	Vector GetVectorSet() {
		return Vec;
	}

	void InputMouseMotion(MotionEvent& Event) {
		if (GetCapture() == Event.CaptureState) {
			mouse.HideCursor();
			GetCapture();
			XMFLOAT2 Delta = mouse.GetMotionDelta(Event.Motion, 0.5);

			//// 헬기 회전 업데이트
			HeliRotation.x += Delta.y;
			HeliRotation.y += Delta.x;
		}
	}

	void InputKey(KeyEvent& Event) {
		switch (Event.Type) {
		case WM_KEYDOWN:
			switch (Event.Key) {
			case 'W': MoveForward = true; break;
			case 'A': MoveLeft = true; break;
			case 'S': MoveBackward = true; break;
			case 'D': MoveRight = true; break;
			case VK_ESCAPE:// 탈출 누를 시 플레이 모드로 전환
				scene.SwitchMode(HomeMode::Start);
				break;
			}
			break;

		case WM_KEYUP:
			switch (Event.Key) {
			case 'W': MoveForward = false; break;
			case 'A': MoveLeft = false; break;
			case 'S': MoveBackward = false; break;
			case 'D': MoveRight = false; break;
			}
			break;
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
		}
	}

	void Update(float FT) {
		//Move
		//if (MoveForward) {
		//	Position.x += Vec.Look.x * FT * 40;
		//	Position.y += Vec.Look.y * FT * 40;
		//	Position.z += Vec.Look.z * FT * 40;
		//}
		//if (MoveBackward || ColBack) {
		//	Position.x -= Vec.Look.x * FT * 40;
		//	Position.y -= Vec.Look.y * FT * 40;
		//	Position.z -= Vec.Look.z * FT * 40;
		//}
		//if (MoveRight) {
		//	Position.x += Vec.Right.x * FT * 40;
		//	Position.y += Vec.Right.y * FT * 40;
		//	Position.z += Vec.Right.z * FT * 40;
		//}
		//if (MoveLeft) {
		//	Position.x -= Vec.Right.x * FT * 40;
		//	Position.y -= Vec.Right.y * FT * 40;
		//	Position.z -= Vec.Right.z * FT * 40;
		//}
		XMFLOAT3 Direction = { 0.0f, 0.0f, 0.0f };

		// 이동 방향 계산
		if (MoveForward) {
			Direction.x += Vec.Look.x;
			Direction.y += Vec.Look.y;
			Direction.z += Vec.Look.z;
		}
		if (MoveBackward || ColBack) {
			Direction.x -= Vec.Look.x;
			Direction.y -= Vec.Look.y;
			Direction.z -= Vec.Look.z;
		}
		if (MoveRight) {
			Direction.x += Vec.Right.x;
			Direction.y += Vec.Right.y;
			Direction.z += Vec.Right.z;
		}
		if (MoveLeft) {
			Direction.x -= Vec.Right.x;
			Direction.y -= Vec.Right.y;
			Direction.z -= Vec.Right.z;
		}

		// 방향 벡터 정규화
		float magnitude = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y + Direction.z * Direction.z);
		if (magnitude > 0.0f) {
			Direction.x /= magnitude;
			Direction.y /= magnitude;
			Direction.z /= magnitude;

			// 이동 적용 (정규화된 방향 * 속도 * 시간)
			Position.x += Direction.x * FT * 40;
			Position.y += Direction.y * FT * 40;
			Position.z += Direction.z * FT * 40;
		}


		//날개 회전
		WingRotation += FT * 2000;
		
	}

	void Render() override {
		//헬기 몸통
		InitRenderState(RENDER_TYPE_3D);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);

		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, 0.0, HeliRotation.z);

		FlipTexture(FLIP_TYPE_V);
		Render3D(HelicopterBodyMesh, HelicopterTex);

		//헬기 머리
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		Render3D(HelicopterHeadMesh, HelicopterTex);

		Math::UpdateVector(Vec, HeliRotation.x, HeliRotation.y, HeliRotation.z);

		//SetFogUse(ENABLE_FOG);

		// 바운드 스페어 출력
		range.Update(Position, 10);
		static int i = 0;
		ColBack = false;
		if (auto enemy_object = scene.Find("enemy"); enemy_object) {
			if (range.CheckCollision(enemy_object->GetRange())) {
				ColBack = true;
				std::cout << i << std::endl;
			}
		}
	}
};