#include "Mode0.h"
#include "MouseUtil.h"

// ���� ȭ�� ���. ������ ǥ���Ѵ�.
#include "Guide.h"

void HomeMode::Start() {
	// ���� ȭ�鿡���� ����� �������� ����
	SetBackgroundColor(0.0, 0.0, 0.0);

	// ���� ��ü �߰�
	scene.AddObject(new Help, "help", LAYER3);

	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);

	scene.RegisterModeName("HomeMode");
}

void HomeMode::Destructor() {
	
}

void HomeMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	KeyEvent Event{ hWnd, nMessageID, wParam, lParam };
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();


	scene.InputKey("help", Event);
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseMotionController(HWND hWnd) {
	MotionEvent Event{ hWnd, mouse.MotionPosition };
	mouse.UpdateMousePosition(hWnd);

	scene.InputMouseMotion("help", Event);

}

// ���콺 ��ư Ŭ�� �� �� ��ũ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	MouseEvent Event{ hWnd, nMessageID, wParam, lParam };

	scene.InputMouse("help", Event);
}