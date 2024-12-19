#pragma once
#include "Config.h"
#include "GameObject.h"
#include "ShaderUtil.h"
#include <vector>
#include <array>
#include <map>

typedef void(*Function)(void);
using LayerIter = std::multimap<const char*, GameObject*>::iterator;
constexpr int Layers = static_cast<int>(END);

class Scene {
private:
	std::array<std::vector<GameObject*>, Layers> ObjectList;
	std::multimap<const char*, GameObject*> ObjectIndex;

	const char* RunningMode{};
	void (*MouseControllerPtr)(HWND, UINT, WPARAM, LPARAM);
	void (*MouseMotionControllerPtr)(HWND);
	void (*KeyboardControllerPtr)(HWND, UINT, WPARAM, LPARAM);

	Function DestructorBuffer{};

protected:
	ID3D12RootSignature* RootSignature{};

public:
	const char* GetMode();
	void RegisterDestructor(Function Destructor);
	void ReleaseDestructor();
	void Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction);
	void SwitchMode(Function ModeFunction);
	void RegisterModeName(const char* ModeName);
	void RegisterKeyController(void(*FunctionPtr)(HWND, UINT, WPARAM, LPARAM));
	void RegisterMouseController(void(*FunctionPtr)(HWND, UINT, WPARAM, LPARAM));
	void RegisterMouseMotionController(void(*FunctionPtr)(HWND));

	void InputKeyMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMotionMessage(HWND hWnd);
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag);
	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag);
	void InputMouseMotion(HWND hWnd, const char* ObjectTag);
	void Exit();
	void Update(float FT);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void UpdateObjectList(int Index);
	void UpdateObjectIndex();
	void AddObject(GameObject*&& Object, const char* Tag, int InputLayer);
	void DeleteObject(GameObject* Object);
	GameObject* Find(const char* Tag);
	std::pair<LayerIter, LayerIter> EqualRange(const char* Tag);
	void ClearAll();
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	ID3D12RootSignature* GetGraphicsRootSignature();
	void ReleaseObjects();
	void PrepareRender(ID3D12GraphicsCommandList* CmdList);
};

// global scope scene
extern Scene scene;