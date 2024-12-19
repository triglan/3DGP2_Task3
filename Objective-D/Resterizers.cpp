#include "ShaderUtil.h"

// �����Ͷ������� ���⿡ �ۼ��Ѵ�.

// �Ϲ� ������ ������
D3D12_RASTERIZER_DESC Shader::CreateRasterizerState() {
	D3D12_RASTERIZER_DESC RasterizerDesc;
	::ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = TRUE;
	RasterizerDesc.AntialiasedLineEnable = TRUE;
	RasterizerDesc.ForcedSampleCount = 0;
	RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(RasterizerDesc);
}

// �ٿ��ڽ� ���� �����Ͷ�����
// ���̾����������� �Ž��� �������ϰ� ���� ó���� ��Ȱ��ȭ �Ѵ�.
D3D12_RASTERIZER_DESC Shader::CreateBoundboxRasterizerState() {
	D3D12_RASTERIZER_DESC RasterizerDesc;
	::ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = TRUE;
	RasterizerDesc.AntialiasedLineEnable = TRUE;
	RasterizerDesc.ForcedSampleCount = 0;
	RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(RasterizerDesc);
}