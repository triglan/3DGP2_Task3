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
void MouseUtil::UpdateMotionPosition(POINT& MotionPosition) {
	::SetCursorPos(MotionPosition.x, MotionPosition.y);
}

// ���콺 ��ǿ� ���� delta���� �����Ѵ�. Sensivity ���� ���� ���� delta���� Ŀ����.
XMFLOAT2 MouseUtil::GetMotionDelta(POINT& MotionPosition, float Sensivity) {
	XMFLOAT2 Delta{};
	Delta.x = (float)(CurrentPosition().x - MotionPosition.x) * Sensivity;
	Delta.y = (float)(CurrentPosition().y - MotionPosition.y) * Sensivity;
	UpdateMotionPosition(MotionPosition);

	return Delta;
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

	x = (((2.0f * (float)ClientPosition.x) / (float)SCREEN_WIDTH) - 1.0f) / camera.StaticProjectionMatrix._11;
	y = 1.0f - (((2.0f * (float)(ClientPosition.y)) / (float)SCREEN_HEIGHT)) / camera.StaticProjectionMatrix._22;
}