#pragma once
#include <DirectXMath.h>
using namespace DirectX;

// ���⿡ CBV�� ����� ����ü�� �����Ѵ�.

// texture flip
struct TEXTURE_FLIP_DATA {
	int X_Flip;
	int Y_Flip;
};

// use light data
struct USE_LIGHT_DATA {
	int UseLight;
};

// light data
struct LIGHT_DATA {
	XMFLOAT3 gLightDirection;  // 12 bytes
	float padding1;                     // 4 bytes (�е��� �߰��Ͽ� 16����Ʈ ����)

	XMFLOAT3 gLightColor;      // 12 bytes
	float padding2;                     // 4 bytes (16����Ʈ ����)

	XMFLOAT3 gAmbientColor;    // 12 bytes
	float gShadowStrength;              // 4 bytes (�̷� ���� 16����Ʈ ���� �Ϸ�)
};

// use fog data
struct USE_FOG_DATA {
	int UseFog;
};

// fog data
struct FOG_DATA {
	DirectX::XMFLOAT3 FogColor; // 12 bytes
	float Padding1;     // 4 bytes

	float FogStart;    // 4 bytes
	XMFLOAT3 Padding2;  // 12 bytes

	float FogEnd;      // 4 bytes
	XMFLOAT3 Padding3;  // 12 bytes
};

