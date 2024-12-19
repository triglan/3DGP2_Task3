#pragma once
#include <DirectXMath.h>
// ���⿡ CBV�� ����� ����ü�� �����Ѵ�.

// texture flip
struct FlipInfo {
	int X_Flip;
	int Y_Flip;
};

// texture alpha
struct AlphaInfo {
	float AlphaValue;
};

// light data
struct LightInfo {
	DirectX::XMFLOAT3 gLightDirection;  // 12 bytes
	float padding1;                     // 4 bytes (�е��� �߰��Ͽ� 16����Ʈ ����)

	DirectX::XMFLOAT3 gLightColor;      // 12 bytes
	float padding2;                     // 4 bytes (16����Ʈ ����)

	DirectX::XMFLOAT3 gAmbientColor;    // 12 bytes
	float gShadowStrength;              // 4 bytes (�̷� ���� 16����Ʈ ���� �Ϸ�)
};

struct UseLightInfo {
	int UseLight;
};