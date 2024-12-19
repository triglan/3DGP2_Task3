#include "MouseUtil.h"
#include "CameraUtil.h"

// Ŀ���� ���� ���·� �����
void MouseUtil::HideCursor() {
	::SetCursor(NULL);
}

// ���콺 ��� ĸ�ĸ� �����Ѵ�
void MouseUtil::StartMotionCapture(HWND hWnd) {
	::SetCapture(hWnd);
	::GetCursorPos(&MotionPosition);
}

// ���콺 ��� ĸ�ĸ� �ߴ��Ѵ�
void MouseUtil::EndMotionCapture() {
	::ReleaseCapture();
}

// ���콺 ��� ��ġ�� ������Ʈ �Ѵ�
void MouseUtil::UpdateMotionPosition(POINT PrevPosition) {
	::SetCursorPos(PrevPosition.x, PrevPosition.y);
}

// ���� Ŀ���� ��ġ�� ��´�
POINT MouseUtil::CurrentPosition() {
	POINT CursorPos;
	::GetCursorPos(&CursorPos);

	return CursorPos;
}

// ���콺 Ŀ�� ��ġ�� ����Ʈ ��ġ�� ��ȯ�Ѵ�.
void MouseUtil::UpdateMousePosition(HWND hWnd) {
	::GetCursorPos(&ClientPosition);
	::ScreenToClient(hWnd, &ClientPosition); // Ŭ���̾�Ʈ ��ǥ�� ��ȯ
	x = (((2.0f * (float)ClientPosition.x) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	y = -(((2.0f * (float)ClientPosition.y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
}