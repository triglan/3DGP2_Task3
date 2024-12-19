#include "ShaderUtil.h"

// ���⿡ ���带 �ۼ��Ѵ�.

// ���带 �����Ѵ�.
D3D12_BLEND_DESC Shader::CreateBlendState() {
	D3D12_BLEND_DESC BlendDesc;
	::ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;

	// 0�� ���� Ÿ���� ���� ����
	BlendDesc.RenderTarget[0].BlendEnable = TRUE; // ���� Ȱ��ȭ
	BlendDesc.RenderTarget[0].LogicOpEnable = FALSE;

	// ���� ���� ���� (SRC_ALPHA * Src + (1 - SRC_ALPHA) * Dest)
	BlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA; // �ҽ� ����
	BlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // �ݴ� ����
	BlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD; // ���ϱ� ����

	// ���� ���� ����
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	// ���� ����ũ ���� (��� ä�� ����)
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//BlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;

	return(BlendDesc);
}
