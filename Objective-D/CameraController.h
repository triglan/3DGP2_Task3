#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "MouseUtil.h"

// 관전 카메라 컨트롤러
class CameraController : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	bool MoveUp{}, MoveDown{};

	XMFLOAT3 CamPosition{0.0, 30.0, -30.0};  // 카메라 위치 기억용

	// 카메라 회전
	XMFLOAT3 CamRotation{};
	XMFLOAT3 DestCamRotation{};

public:
	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (camera.CurrentMode() != CamMode::TRACK_MODE) {
			switch (nMessageID) {
			case WM_KEYDOWN:
				switch (wParam) {
				case 'W': MoveForward = true; break;
				case 'A': MoveLeft = true; break;
				case 'S': MoveBackward = true; break;
				case 'D': MoveRight = true; break;
				case VK_SPACE: MoveUp = true; break;
				case VK_CONTROL: MoveDown = true; break;

				}
				break;

			case WM_KEYUP:
				switch (wParam) {
				case 'W': MoveForward = false; break;
				case 'A': MoveLeft = false; break;
				case 'S': MoveBackward = false; break;
				case 'D': MoveRight = false; break;
				case VK_SPACE: MoveUp = false; break;
				case VK_CONTROL: MoveDown = false; break;
				}
				break;
			}
		}
	}

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (GetCapture() == hWnd) {
			mouse.HideCursor();
		}
	}

	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (camera.CurrentMode() != CamMode::TRACK_MODE) {
			switch (nMessageID) {
			case WM_LBUTTONDOWN:
				mouse.StartMotionCapture(hWnd);
				break;

			case WM_LBUTTONUP:
				mouse.EndMotionCapture();
				break;
			}
		}
	}

	void Update(float FT) {
		if (camera.CurrentMode() != CamMode::TRACK_MODE) {
			// 부드러운 카메라 회전
			CamRotation.x = std::lerp(CamRotation.x, DestCamRotation.x, FT * 15);
			CamRotation.y = std::lerp(CamRotation.y, DestCamRotation.y, FT * 15);

			camera.Rotate(CamRotation.x, CamRotation.y, 0.0);

			CamPosition = camera.GetPosition();
		}
		//}
	}
};