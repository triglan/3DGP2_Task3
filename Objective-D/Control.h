#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

#include "LineBrush.h"
#include "Mode0.h"

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
	OOBB GetRANGE;

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

	void InputMouseMotion(HWND hwnd, POINT MotionPosition) {
		if (GetCapture() == hwnd) {
			mouse.HideCursor();
			GetCapture();

			float cxDelta = (float)(mouse.CurrentPosition().x - MotionPosition.x) / 5.0f;
			float cyDelta = (float)(mouse.CurrentPosition().y - MotionPosition.y) / 5.0f;
			mouse.UpdateMotionPosition(MotionPosition);
			
			//// 헬기 회전 업데이트
			HeliRotation.x += cyDelta;
			HeliRotation.y += cxDelta;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
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
			switch (wParam) {
			case 'W': MoveForward = false; break;
			case 'A': MoveLeft = false; break;
			case 'S': MoveBackward = false; break;
			case 'D': MoveRight = false; break;
			}
			break;
		}
	}

	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			mouse.StartMotionCapture(hWnd);
			break;

		case WM_LBUTTONUP:
			mouse.EndMotionCapture();
			break;
		}
	}

	void Update(float FT) {


		//Move
		if (MoveForward) {
			Position.x += Vec.Look.x * FT * 40;
			Position.y += Vec.Look.y * FT * 40;
			Position.z += Vec.Look.z * FT * 40;
		}
		if (MoveBackward || ColBack) {
			Position.x -= Vec.Look.x * FT * 40;
			Position.y -= Vec.Look.y * FT * 40;
			Position.z -= Vec.Look.z * FT * 40;
		}
		if (MoveRight) {
			Position.x += Vec.Right.x * FT * 40;
			Position.y += Vec.Right.y * FT * 40;
			Position.z += Vec.Right.z * FT * 40;
		}
		if (MoveLeft) {
			Position.x -= Vec.Right.x * FT * 40;
			Position.y -= Vec.Right.y * FT * 40;
			Position.z -= Vec.Right.z * FT * 40;
		}
		//날개 회전
		WingRotation += FT * 2000;
		
	}

	void Render(CommandList CmdList) {
		//헬기 몸통
		InitMatrix(CmdList, RENDER_TYPE_PERS);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);

		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, 0.0, HeliRotation.z);

		FlipTexture(CmdList, FLIP_TYPE_V);
		RenderMesh(CmdList, HelicopterBodyMesh, HelicopterTex, ObjectShader);

		//헬기 머리
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterHeadMesh, HelicopterTex, ObjectShader);

		Math::UpdateVector(Vec, HeliRotation.x, HeliRotation.y, HeliRotation.z);


		//스카이박스
		//InitMatrix(CmdList, RENDER_TYPE_PERS);
		//Transform::Scale(ScaleMatrix, 500, 500, 500);
		//RenderMesh(CmdList, SkyboxMesh, SkyboxTex, ObjectShader, 1.0f);

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
		//range.Render(CmdList);


	}
};