#include "RootConstantUtil.h"

// ��Ʈ����� ���̴��� �����Ѵ�.
void RCUtil::Input(ID3D12GraphicsCommandList* CmdList, void* Data, int RootSignatureIndex, int BitValue, int Offset) {
	CmdList->SetGraphicsRoot32BitConstants(RootSignatureIndex, BitValue, Data, Offset);
}