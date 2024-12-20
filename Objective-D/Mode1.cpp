#include "Mode1.h"
#include "MouseUtil.h"
#include <random>

#include "TestObject.h"//--
#include "Control.h"
#include "enemy.h"
#include "Terrain.h"
#include "CameraController.h"
#include "Tree.h"
#include "Skybox.h"

// �ش� cpp���ϰ� h������ Ư���� ��带 �����ϰ�, �ش� ��忡 �����ϴ� ��ü���� ��Ʈ���ϱ� ���� �����̴�.
// �ݵ�� cpp, h���Ϸ� �и��Ǿ��־�� �ϸ�, �� ��忡 ���� ���ӽ����̽��� ���еǾ���Ѵ�.
// ���Ӹ��::Start() ������ ��� �Լ� ���ӽ����̽��� ��� �̸��� �ۼ��ϴ� ���� �����Ѵ�
// �Ʒ��� ���� ������ ��� ���� �Լ� �� ��Ʈ�ѷ� �Լ��� �ۼ����ֱ⸸ �ϸ� �����ӿ�ũ�� �˾Ƽ� ���ش�.
// ���α׷� ���� �� �Ʒ��� �Լ����� [ DirectX 3D ] -> [ Main ] -> Scene.cpp�� �ִ� Init() �Լ����� ����ȴ�.
// �Ǵ� ��� ���� �� ����ȴ�.
// ���� ������ Scene.cpp���� �����ϵ��� �Ѵ�.

// ������ ��� �ڵ� �ۼ��� ���� [ Template ] ���Ϳ� ���ø��� ����� �ξ����� ������ �� �Լ� �̸��� ���ӽ����̽� �̸��� �ٲٸ� �ȴ�.

void Mode1::Start() {
	SetBackgroundColor(0.5, 0.5, 0.5);
	//scene.AddObject(new TestObject, "test_object", LAYER1);

	scene.AddObject(new CameraController, "cam_controller", LAYER1);
	scene.AddObject(new Control, "control", LAYER1);
	scene.AddObject(new Enemy, "enemy", LAYER1);
	scene.AddObject(new Skybox, "skybox", LAYER1);
	for (int i = 0; i < 25; ++i)// x,z : -240 ~ 240
		scene.AddObject(new Terrain(i / 5 * 120 - 240, i % 5 * 120 - 240), "terrain", LAYER1);

	std::random_device rd{};
	std::uniform_real_distribution<float> DistX(-200.0, 200.0);
	std::uniform_real_distribution<float> DistY(-200.0, 200.0);

	for (int i = 0; i < 100; ++i) {
		float RandX, RandZ;
		RandX = DistX(rd);
		RandZ = DistY(rd);
		scene.AddObject(new Tree(RandX, RandZ), "tree", LAYER1);
	}

	// scene�� ��Ʈ�ѷ� �� ��� �Ҹ��� ���
	RegisterController();

	// ��� �̸��� Scene�� ���
	scene.RegisterModeName("Mode1");
}

void Mode1::Destructor() {
	//scene.
}

void Mode1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	KeyEvent Event{ hWnd, nMessageID, wParam, lParam };

	// esc ���� �� ���α׷� ����
	if (Event.Type == WM_KEYDOWN && Event.Key == VK_ESCAPE)
		scene.SwitchMode(HomeMode::Start);

	scene.InputKey("control", Event);
	scene.InputKey("cam_controller", Event);
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseMotionController(HWND hWnd) {
	MotionEvent Event{ hWnd, mouse.MotionPosition };

	// ���콺 ��ǥ�� ����Ʈ ��ǥ�� ��ȯ�Ѵ�.
	mouse.UpdateMousePosition(hWnd);

	// ��ü�� ���콺 ��� �Է�
	//scene.InputMouseMotion("test_object", Event);//--
	scene.InputMouseMotion("control", Event);
	scene.InputMouseMotion("cam_controller", Event);
}

// ���콺 ��ư Ŭ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	MouseEvent Event{ hWnd, nMessageID, wParam, lParam };

	// ��ü�� ���콺 �Է�
	//--scene.InputMouse("test_object", Event);
	scene.InputMouse("control", Event);
	scene.InputMouse("cam_controller", Event);
}

// scene�� ��Ʈ�ѷ� �� ��� �Ҹ��� ���
void Mode1::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}