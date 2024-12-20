#include "Mode0.h"
#include "MouseUtil.h"

// 시작 화면 모드. 도움말을 표시한다.
#include "Guide.h"

void HomeMode::Start() {
	// 도움말 화면에서는 배경을 검정으로 변경
	SetBackgroundColor(0.0, 0.0, 0.0);

	// 도움말 객체 추가
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
	// esc 누를 시 프로그램 종료
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();


	scene.InputKey("help", Event);
}

//  마우스 모션을 지정된 객체 포인터로 전달한다
void HomeMode::MouseMotionController(HWND hWnd) {
	MotionEvent Event{ hWnd, mouse.MotionPosition };
	mouse.UpdateMousePosition(hWnd);

	scene.InputMouseMotion("help", Event);

}

// 마우스 버튼 클릭 및 휠 스크롤 이벤트를 지정된 객체 포인터로 전달한다
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	MouseEvent Event{ hWnd, nMessageID, wParam, lParam };

	scene.InputMouse("help", Event);
}