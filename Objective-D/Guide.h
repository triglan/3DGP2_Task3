#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include <cmath>

#include "Mode1.h"

// 도움말
class Help : public GameObject {
private:
	// 실제 스크롤 위치
	float ScrollY = -1.0;

	// 스크롤 위치 목표
	float ScrollDest = -1.0;

	// 스크롤 안내 화살표 위치
	float ArrowPosition{};
	float Num{};

	// press to enter 안내문 투명도
	float EnterAlpha{};
	float AlphaNum{};

public:
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_MOUSEWHEEL:{
			int ScrollVar = GET_WHEEL_DELTA_WPARAM(wParam);

			// 위로 스크롤
			if (ScrollVar > 0 && ScrollDest > -1.0)
				ScrollDest -= 0.1;

			// 아애로 스크롤
			else if (ScrollVar < 0 && ScrollDest < 1.5)
				ScrollDest += 0.1;
		}
			break;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_RETURN:
				// 엔터 누를 시 플레이 모드로 전환
				scene.SwitchMode(Mode1::Start);
				break;
			}

		}
	}

	void Update(float FT) {
		//  lerp로 부드러운 스크롤을 구현한다
		ScrollY = std::lerp(ScrollY, ScrollDest, FT * 10);

		// 스크롤 안내 화살표 애니메이션
		Num += FT * 5;
		ArrowPosition = sin(Num) * 0.1;

		// press to enter 애니메이션
		AlphaNum += FT * 2;
		EnterAlpha = sin(AlphaNum);
		if (EnterAlpha < 0.0) {
			EnterAlpha = 0.0;
			AlphaNum = 0.0;
		}
	}

	void Render(CommandList CmdList) {
		//메뉴 띄우기
		InitRenderState(RENDER_TYPE_2D);//??
		// 이미지 종횡비와 동일하게 매쉬의 종횡비를 조정
		Transform::ImageAspect(ScaleMatrix, 1300, 800);  // 이미지 크기에 맞게 수정
		Transform::Scale(ScaleMatrix, 3.0, 3.0, 3.0);   // 스케일링 값 조정 가능

		// 텍스처 바인딩 (MyImageTex를 사용하여 PNG 이미지 출력)
		Render2D(WoodTex);
	}
};