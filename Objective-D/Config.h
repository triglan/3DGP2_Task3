#pragma once
#include <tchar.h>

// ī�޶� ��� ����
// ī�޶� Ŭ���� ��� �Լ� Update(float FT)���� ������ �� ����ϴ� ������ Ŭ�����̴�. ī�޶� Ŭ�������� �ڼ��� �����ϵ��� �Ѵ�.
enum class CamMode{ TRACK_MODE, SPECTOR_MODE };

// ���α׷� ���� �� ����� ������ ����� �����Ѵ�.
// START_WITH_FULL_SCREEN �ɼ� Ȱ��ȭ �� ����� �ػ󵵷� ����ȴ�.
constexpr int START_UP_WIDTH = 1300;
constexpr int START_UP_HEIGHT = 800;

// Ǯ��ũ�� ���� ������ ���� �����Ѵ�.
constexpr bool START_WITH_FULL_SCREEN = false;

// �ִ� ������ ������ �����Ѵ�. 0���� ���� �� �ִ� ���������� �����Ѵ�.
constexpr float FRAME_LIMITS = 144;

// �ٿ��ڽ� ǥ�� ���θ� �����Ѵ�.
#define SHOW_BOUND_BOX

// ���� ���忡 ����� ���̾ �����Ѵ�. �� �κп��� �ݵ�� ENDǥ�⸦ �ؾ��Ѵ�.
// UI�� ��� ���� �ֻ�� ���̾ �߰��ϴ� ���� �����Ѵ�.
enum LayerEnum
{ LAYER_1, LAYER_2, LAYER_3, END };

// Scene �����̳� ������ ������ ����
constexpr int OBJECT_LIST_RESERVE = 500;

// ����ǥ���� ���ڿ��� �����Ѵ�. �ݵ�� �Ʒ��� ���� �������� �����ؾ� �Ѵ�.
constexpr TCHAR TitleName[] = _T("Objective-D(");