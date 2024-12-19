#include "ObjectShader.h"

// ������Ʈ ��¿� ���̴�
D3D12_INPUT_LAYOUT_DESC Object_Shader::CreateInputLayout() {
	UINT NumInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* InputElementDescs = new D3D12_INPUT_ELEMENT_DESC[NumInputElementDescs];

	InputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[2] = { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;
	InputLayoutDesc.pInputElementDescs = InputElementDescs;
	InputLayoutDesc.NumElements = NumInputElementDescs;

	return(InputLayoutDesc);
}

D3D12_SHADER_BYTECODE Object_Shader::CreateVertexShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//SystemResources//Shader//ObjectShader.hlsl", "VSTexColor", "vs_5_1", ShaderBlob));
}

D3D12_SHADER_BYTECODE Object_Shader::CreatePixelShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//SystemResources//Shader//ObjectShader.hlsl", "PSTexColor", "ps_5_1", ShaderBlob));
}