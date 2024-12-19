#include "LineShader.h"

// ������Ʈ ��¿� ���̴�
D3D12_INPUT_LAYOUT_DESC Line_Shader::CreateInputLayout() {
	UINT NumInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* InputElementDescs = new D3D12_INPUT_ELEMENT_DESC[NumInputElementDescs];

	InputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[1] = { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;
	InputLayoutDesc.pInputElementDescs = InputElementDescs;
	InputLayoutDesc.NumElements = NumInputElementDescs;

	return(InputLayoutDesc);
}

D3D12_SHADER_BYTECODE Line_Shader::CreateVertexShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//SystemResources//Shader//LineShader.hlsl", "VSLineColor", "vs_5_1", ShaderBlob));
}

D3D12_SHADER_BYTECODE Line_Shader::CreatePixelShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//SystemResources//Shader//LineShader.hlsl", "PSLineColor", "ps_5_1", ShaderBlob));
}