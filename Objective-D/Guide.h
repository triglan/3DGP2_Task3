#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include <cmath>

#include "Mode1.h"

// 도움말
class Help : public GameObject {
private:

public:
	void InputMouse(MouseEvent& Event) {
		switch (Event.Type) {
		}
	}

	void InputKey(KeyEvent& Event) {
		switch (Event.Type) {
		case WM_KEYDOWN:
			switch (Event.Key) {
			case VK_RETURN: case VK_SPACE:
				scene.SwitchMode(Mode1::Start);
				break;
			}
		}
	}

	void Update(float FT) {

	}

	void Render() override {
		//메뉴 띄우기
		InitRenderState(RENDER_TYPE_2D);
		
		Transform::ImageAspect(ScaleMatrix, 1000, 800); 
		Transform::Scale(ScaleMatrix, 3.0, 3.0, 3.0);   

		Render2D(GuideTex);
	}
};