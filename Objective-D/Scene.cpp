#include "Scene.h"
#include "GameResource.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "MouseUtil.h"

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Scene::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// ��Ʈ �ñ״�ó�� �����Ѵ�.
	RootSignature = CreateGraphicsRootSignature(Device);

	// ���� ���̴��� �����Ѵ�.
	CreateShaderResource(RootSignature, Device, CmdList);

	// ���� �Ž��� �����Ѵ�.
	CreateMeshResource(Device, CmdList);

	// ���� �ؽ�ó�� �����Ѵ�
	CreateTextureResource(Device, CmdList);

	// Config.h���� ������ ������ ��ŭ �� ���̾� ������
	for (int i = 0; i < Layers; ++i)
		ObjectList[i].reserve(OBJECT_LIST_RESERVE);

	ModeFunction();
}

// ���� ���� ���� ��� �̸��� �����Ѵ�
const char* Scene::GetMode() {
	return RunningMode;
}

void Scene::RegisterDestructor(Function Destructor) {
	DestructorBuffer = Destructor;
}

void Scene::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

// ��带 �����Ѵ�. ��� ���� �� ���� ��忡 �ִ� ��ü���� ��� �����ȴ�.
void Scene::SwitchMode(Function ModeFunction) {
	ClearAll();
	if (DestructorBuffer)
		DestructorBuffer();
	ModeFunction();
}

void Scene::RegisterModeName(const char* ModeName) {
	RunningMode = ModeName;
}

// ��Ʈ�ѷ� ���� �Լ��̴�. �� �Լ��� ���� �ۼ��� ���� ����,
void Scene::RegisterKeyController(void (*FunctionPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardControllerPtr = FunctionPtr;
}
void Scene::RegisterMouseController(void (*FunctionPtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseControllerPtr = FunctionPtr;
}
void Scene::RegisterMouseMotionController(void (*FunctionPtr)(HWND)) {
	MouseMotionControllerPtr = FunctionPtr;
}

// Ű����, ���콺, ���콺 �������� WinMain���κ��� �޾ƿ´�. ���� �� ���� ����.
void Scene::InputKeyMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (KeyboardControllerPtr)
		KeyboardControllerPtr(hWnd, nMessageID, wParam, lParam);
}

void Scene::InputMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (MouseControllerPtr)
		MouseControllerPtr(hWnd, nMessageID, wParam, lParam);
}

void Scene::InputMouseMotionMessage(HWND hWnd) {
	if (MouseMotionControllerPtr)
		MouseMotionControllerPtr(hWnd);
}

// ��ü�� ã�� ��Ʈ�ѷ� �Լ��� �޽����� �����Ѵ�. �ټ��� �����ϴ� ��ü�� ������� �ʵ��� �����Ѵ�.
void Scene::InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag) {
	auto It = ObjectIndex.find(ObjectTag);
	if (It != end(ObjectIndex) && !It->second->DeleteMark)
		It->second->InputMouse(hWnd, nMessageID, wParam, lParam);
}

void Scene::InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag) {
	auto It = ObjectIndex.find(ObjectTag);
	if (It != end(ObjectIndex) && !It->second->DeleteMark)
		It->second->InputKey(hWnd, nMessageID, wParam, lParam);
}

void Scene::InputMouseMotion(HWND hWnd,  const char* ObjectTag) {
	auto It = ObjectIndex.find(ObjectTag);
	if (It != end(ObjectIndex) && !It->second->DeleteMark)
		It->second->InputMouseMotion(hWnd, mouse.MotionPosition);
}

// ���α׷��� �����Ѵ�.
void Scene::Exit() {
	PostQuitMessage(1);
}

// ���� �����ϴ� ��� ��ü���� ������Ʈ�Ѵ�
// ���� ��ũ�� ǥ�õ� ��ü�� ������Ʈ���� �ʴ´�
void Scene::Update(float FT) {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteMark)
				O->Update(FT);
		}
		UpdateObjectList(i);
	}
}

// ���� �����ϴ� ��� ��ü���� �������Ѵ�
// ���� ��ũ�� ǥ�õ� ��ü���� ���������� �ʴ´�.
void Scene::Render(ID3D12GraphicsCommandList* CmdList) {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteMark)
				O->Render(CmdList);
		}
	}
}

// ��ü�� �߰��Ѵ�. ���ϴ� ��ü�� �±�, ���̾ ������ �� �ִ�.
// �� �Լ����� �Է��� �±״� Find()�Լ����� ���ȴ�.
void Scene::AddObject(GameObject*&& Object, const char* Tag, int InputLayer) {
	ObjectList[InputLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));
	Object->ObjectTag = Tag;
}

// �����͸� ����Ͽ� ��ü�� �����Ѵ�. ��ü�� ���� ��ũ�� ǥ���Ѵ�.
// �� �ڵ尡 ����Ǵ� ������ ��� �������� ������ �����Ѵ�.
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateObjectIndex()���� ���������� �����ȴ�.
// Ŭ���� ���ο��� this �����ͷε� �ڽ��� ������ �� �ִ�.
void Scene::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// ���� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
GameObject* Scene::Find(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != std::end(ObjectIndex) && !It->second->DeleteMark)
		return It->second;
	else
		return nullptr;
}

// Ư�� �±׸� ���� ������Ʈ���� ������ ������ �����Ѵ�.
// �ش� �Լ��� equal range�� ��� for������ �����ϸ� �ȴ�.
std::pair<LayerIter, LayerIter> Scene::EqualRange(const char* Tag) {
	return ObjectIndex.equal_range(Tag);
}

// ���� �����ϴ� ��� ��ü���� �����Ѵ�.
void Scene::ClearAll() {
	for (const auto& O : ObjectIndex)
		O.second->DeleteMark = true;
}

// ���� ��ũ�� ǥ�õ� ��ü���� �����̳ʿ��� �����Ѵ�.
// ���� ��ü ������ �ƴԿ� ����
void Scene::UpdateObjectList(int Index) {
	for (auto It = begin(ObjectList[Index]); It != end(ObjectList[Index]);) {
		if ((*It)->DeleteMark) {
			It = ObjectList[Index].erase(It);
			continue;
		}
		++It;
	}
}

// ���� ��ũ�� ǥ�õ� ��ü���� ������ �����Ѵ�.
void Scene::UpdateObjectIndex() {
	for (auto It = begin(ObjectIndex); It != end(ObjectIndex);) {
		if (It->second->DeleteMark) {
			delete It->second;
			It->second = nullptr;
			It = ObjectIndex.erase(It);
			continue;
		}
		++It;
	}
}

/////////////////////

void SetRoot(std::vector<D3D12_ROOT_PARAMETER>& RootParam, int NumValue, int RegisterNum, int RootIndex, int& IndexValue) {
	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParam[RootIndex].Constants.Num32BitValues = NumValue;
	RootParam[RootIndex].Constants.ShaderRegister = RegisterNum;
	RootParam[RootIndex].Constants.RegisterSpace = 0;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	IndexValue = RootIndex;
}

void SetCBV(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex, CBV& CBV_Struct) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	CBVUtil::SetSignatureIndex(CBV_Struct, RootIndex);
}

void SetSRV(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex, int& IndexValue) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	IndexValue = RootIndex;
}

void SetSampler(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex, int& IndexValue) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	IndexValue = RootIndex;
}

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* Device) {
	ID3D12RootSignature* GraphicsRootSignature = NULL;

	// 32��Ʈ ������� ����
	D3D12_DESCRIPTOR_RANGE Range{};

	// ��Ʈ �Ķ����
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(9);

	int Dummy{};
	SetRoot(RootParameters, 0, 0, 0, Dummy);                // b0, cbFrameworkInfo | �̻���̶� ��Ʈ��� ���Ҵ�

	SetRoot(RootParameters, 19, 1, 1, GAME_OBJECT_INDEX);   // b1, cbGameObjectInfo

	SetRoot(RootParameters, 35, 2, 2, CAMERA_INDEX);        // b2, cbCameraInfo

	SetCBV(Range, RootParameters, 3, 3, FlipCBV);           // b3, cbFlipInfo

	SetRoot(RootParameters, 1, 4, 4, ALPHA_INDEX);          // b4, cbAlphaInfo

	SetCBV(Range, RootParameters, 5, 5, LightCBV);          // b5, cbLightInfo

	SetCBV(Range, RootParameters, 6, 6, BoolLightCBV);      // b6, cbLightUseInfo

	SetSRV(Range, RootParameters, 0, 7, SRV_INDEX);         // t0, SRV

	SetSampler(Range, RootParameters, 0, 8, SAMPLER_INDEX); // s0, Sampler

	// ��Ʈ �ñ״�ó ����
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
	RootSignatureDesc.NumParameters = RootParameters.size(); //_countof(RootParameters);
	RootSignatureDesc.pParameters = RootParameters.data();
	RootSignatureDesc.NumStaticSamplers = 0;
	RootSignatureDesc.pStaticSamplers = NULL;
	RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* SignatureBlob = NULL;
	ID3DBlob* ErrorBlob = NULL;
	D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SignatureBlob, &ErrorBlob);
	Device->CreateRootSignature(0, SignatureBlob->GetBufferPointer(), SignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&GraphicsRootSignature);

	if (SignatureBlob) SignatureBlob->Release();
	if (ErrorBlob) ErrorBlob->Release();

	return GraphicsRootSignature;
}

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Scene::GetGraphicsRootSignature() {
	return(RootSignature);
}

// ��Ʈ�ñ״�ó�� �������Ѵ�
void Scene::ReleaseObjects() {
	if (RootSignature)
		RootSignature->Release();
}

// �������� �غ��Ѵ�
void Scene::PrepareRender(ID3D12GraphicsCommandList* CmdList) {
	CmdList->SetGraphicsRootSignature(RootSignature);
}