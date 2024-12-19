#include "Mode0.h"
#include "MouseUtil.h"

// ���� ȭ�� ���. ������ ǥ���Ѵ�.
#include "Guide.h"

void HomeMode::Start() {
	// ���� ȭ�鿡���� ����� �������� ����
	SetBackgroundColor(0.0, 0.0, 0.0);

	// ���� ��ü �߰�
	scene.AddObject(new Help, "help", LAYER_3);

	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("HomeMode");
}

void HomeMode::Destructor() {
	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (���ҽ� �޸� ���� ��)
}

void HomeMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// ���α׷��� �����ϴ� �����ӿ�ũ ��� �Լ�
		scene.Exit();

	scene. InputKey(hWnd, nMessageID, wParam, lParam, "help");
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseMotionController(HWND hWnd) {
	// ���콺 ��ǥ�� ����Ʈ ��ǥ�� ��ȯ�Ѵ�.
	mouse.UpdateMousePosition(hWnd);
}

// ���콺 ��ư Ŭ�� �� �� ��ũ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// ��Ŭ��, ��Ŭ�� ���� ���¸� ������Ʈ �Ѵ�
	scene.InputMouse(hWnd, nMessageID, wParam, lParam, "help");
}