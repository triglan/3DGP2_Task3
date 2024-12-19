#include "Mode1.h"
#include "MouseUtil.h"

#include "TestObject.h"

// �ش� cpp���ϰ� h������ Ư���� ��带 �����ϰ�, �ش� ��忡 �����ϴ� ��ü���� ��Ʈ���ϱ� ���� �����̴�.
// �ݵ�� cpp, h���Ϸ� �и��Ǿ��־�� �ϸ�, �� ��忡 ���� ���ӽ����̽��� ���еǾ���Ѵ�.
// ���Ӹ��::Start() ������ ��� �Լ� ���ӽ����̽��� ��� �̸��� �ۼ��ϴ� ���� �����Ѵ�
// �Ʒ��� ���� ������ ��� ���� �Լ� �� ��Ʈ�ѷ� �Լ��� �ۼ����ֱ⸸ �ϸ� �����ӿ�ũ�� �˾Ƽ� ���ش�.
// ���α׷� ���� �� �Ʒ��� �Լ����� [ DirectX 3D ] -> [ Main ] -> Scene.cpp�� �ִ� Init() �Լ����� ����ȴ�.
// �Ǵ� ��� ���� �� ����ȴ�.
// ���� ������ Scene.cpp���� �����ϵ��� �Ѵ�.

// ������ ��� �ڵ� �ۼ��� ���� [ Template ] ���Ϳ� ���ø��� ����� �ξ����� ������ �� �Լ� �̸��� ���ӽ����̽� �̸��� �ٲٸ� �ȴ�.

void Mode1::Start() {
	// �ʿ��� ��ü �߰�
	scene.AddObject(new TestObject, "test_object", LAYER1);
	
	// scene�� ��Ʈ�ѷ� �� ��� �Ҹ��� ���
	RegisterController();

	// ��� �̸��� Scene�� ���
	scene.RegisterModeName("Mode1");
}

void Mode1::Destructor() {
	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (���ҽ� �޸� ���� ��)
}

void Mode1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	KeyEvent Event{ hWnd, nMessageID, wParam, lParam };

	// esc ���� �� ���α׷� ����
	if (Event.Type == WM_KEYDOWN && Event.Key == VK_ESCAPE)
		// ���α׷��� �����ϴ� Scene ��� �Լ�
		scene.Exit();
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseMotionController(HWND hWnd) {
	MotionEvent Event{ hWnd, mouse.MotionPosition };

	// ���콺 ��ǥ�� ����Ʈ ��ǥ�� ��ȯ�Ѵ�.
	mouse.UpdateMousePosition(hWnd);

	// ��ü�� ���콺 ��� �Է�
	scene.InputMouseMotion("test_object", Event);
}

// ���콺 ��ư Ŭ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	MouseEvent Event{ hWnd, nMessageID, wParam, lParam };

	// ��ü�� ���콺 �Է�
	scene.InputMouse("test_object", Event);
}

// scene�� ��Ʈ�ѷ� �� ��� �Ҹ��� ���
void Mode1::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}