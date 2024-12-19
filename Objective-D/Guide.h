#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include <cmath>

#include "Mode1.h"

// ����
class Help : public GameObject {
private:
	// ���� ��ũ�� ��ġ
	float ScrollY = -1.0;

	// ��ũ�� ��ġ ��ǥ
	float ScrollDest = -1.0;

	// ��ũ�� �ȳ� ȭ��ǥ ��ġ
	float ArrowPosition{};
	float Num{};

	// press to enter �ȳ��� ����
	float EnterAlpha{};
	float AlphaNum{};

public:
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_MOUSEWHEEL:{
			int ScrollVar = GET_WHEEL_DELTA_WPARAM(wParam);

			// ���� ��ũ��
			if (ScrollVar > 0 && ScrollDest > -1.0)
				ScrollDest -= 0.1;

			// �ƾַ� ��ũ��
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
				// ���� ���� �� �÷��� ���� ��ȯ
				scene.SwitchMode(Mode1::Start);
				break;
			}

		}
	}

	void Update(float FT) {
		//  lerp�� �ε巯�� ��ũ���� �����Ѵ�
		ScrollY = std::lerp(ScrollY, ScrollDest, FT * 10);

		// ��ũ�� �ȳ� ȭ��ǥ �ִϸ��̼�
		Num += FT * 5;
		ArrowPosition = sin(Num) * 0.1;

		// press to enter �ִϸ��̼�
		AlphaNum += FT * 2;
		EnterAlpha = sin(AlphaNum);
		if (EnterAlpha < 0.0) {
			EnterAlpha = 0.0;
			AlphaNum = 0.0;
		}
	}

	void Render(CommandList CmdList) {
		//�޴� ����
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);//??
		// �̹��� ��Ⱦ��� �����ϰ� �Ž��� ��Ⱦ�� ����
		Transform::ImageAspect(ScaleMatrix, 1300, 800);  // �̹��� ũ�⿡ �°� ����
		Transform::Scale(ScaleMatrix, 3.0, 3.0, 3.0);   // �����ϸ� �� ���� ����

		// �ؽ�ó ���ε� (MyImageTex�� ����Ͽ� PNG �̹��� ���)
		RenderMesh(CmdList, ImagePannel, GuideTex, ObjectShader);

	}
};