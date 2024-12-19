#pragma once
#include "DirectX_3D.h"

// ���콺�� ����ϴ� ��ƿ�̴�. 

enum class Type
{LEFT_BUTTON, RIGHT_BUTTON};

enum class State
{BUTTON_DOWN, BUTTON_UP};

class MouseUtil {
public:
	float x{}, y{};
	POINT MotionPosition{};
	POINT ClientPosition{};
	bool LBUTTONDOWN{}, RBUTTONDOWN{};

	void HideCursor();
	void StartMotionCapture(HWND hWnd);
	void EndMotionCapture();
	void UpdateMotionPosition(POINT PrevPosition);
	POINT CurrentPosition();
	void UpdateMousePosition(HWND hWnd);
};
extern MouseUtil mouse;
