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

// 해당 cpp파일과 h파일은 특정한 모드를 실행하고, 해당 모드에 존재하는 객체들을 컨트롤하기 위한 파일이다.
// 반드시 cpp, h파일로 분리되어있어야 하며, 각 모드에 따라 네임스페이스로 구분되어야한다.
// 게임모드::Start() 식으로 모드 함수 네임스페이스와 모드 이름을 작성하는 것을 권장한다
// 아래와 같은 형태의 모드 실행 함수 및 컨트롤러 함수를 작성해주기만 하면 프레임워크가 알아서 해준다.
// 프로그램 시작 시 아래의 함수들은 [ DirectX 3D ] -> [ Main ] -> Scene.cpp에 있는 Init() 함수에서 실행된다.
// 또는 모드 변경 시 실행된다.
// 관련 내용은 Scene.cpp에서 설명하도록 한다.

// 간편한 모드 코드 작성을 위해 [ Template ] 필터에 템플릿을 만들어 두었으니 복붙한 후 함수 이름과 네임스페이스 이름을 바꾸면 된다.

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

	// scene에 컨트롤러 및 모드 소멸자 등록
	RegisterController();

	// 모드 이름을 Scene에 등록
	scene.RegisterModeName("Mode1");
}

void Mode1::Destructor() {
	//scene.
}

void Mode1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	KeyEvent Event{ hWnd, nMessageID, wParam, lParam };

	// esc 누를 시 프로그램 종료
	if (Event.Type == WM_KEYDOWN && Event.Key == VK_ESCAPE)
		scene.SwitchMode(HomeMode::Start);

	scene.InputKey("control", Event);
	scene.InputKey("cam_controller", Event);
}

//  마우스 모션을 지정된 객체 포인터로 전달한다
void Mode1::MouseMotionController(HWND hWnd) {
	MotionEvent Event{ hWnd, mouse.MotionPosition };

	// 마우스 좌표를 뷰포트 좌표로 변환한다.
	mouse.UpdateMousePosition(hWnd);

	// 객체로 마우스 모션 입력
	//scene.InputMouseMotion("test_object", Event);//--
	scene.InputMouseMotion("control", Event);
	scene.InputMouseMotion("cam_controller", Event);
}

// 마우스 버튼 클릭 이벤트를 지정된 객체 포인터로 전달한다
void Mode1::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	MouseEvent Event{ hWnd, nMessageID, wParam, lParam };

	// 객체로 마우스 입력
	//--scene.InputMouse("test_object", Event);
	scene.InputMouse("control", Event);
	scene.InputMouse("cam_controller", Event);
}

// scene에 컨트롤러 및 모드 소멸자 등록
void Mode1::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}