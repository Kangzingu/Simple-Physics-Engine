#pragma once
#include "Graphics/AdapterReader.h"
#include "Graphics/Camera.h"
#include "Graphics/ForceGenerator.h"
#include "Graphics/ImGui/imgui.h"
#include "Graphics/ImGui/imgui_impl_win32.h"
#include "Graphics/ImGui/imgui_impl_dx11.h"
#include "Graphics/Light.h"
#include "Graphics/Actor.h"
#include "Graphics/Shaders.h"
#include "Utils/Timer.h"
#include "Utils/WIndow/WindowManager.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

using namespace std;
using namespace Microsoft::WRL;

class Engine
{
public:
	void Initialize(HINSTANCE hInstance);
	void InitializeWindow(HINSTANCE hInstance);
	void InitializeDirectX();
	void InitializeShaders();
	void InitializeScene();

	void Run();
	void HandleEvent();
	void UpdatePhysics();
	void UpdateTimer();
	void UpdateScene();
	void UpdateUI();
	bool IsRenderWindowExist();

private:
	Light light;
	Camera camera;
	vector<Actor> actors;

	Timer fpsTimer;
	Timer sceneTimer;
	string fps = "FPS: 0";
	int fpsCount = 0;
	float deltaTime = 0;

	WindowManager windowManager;
	ForceGenerator forceGenerator;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;// 그리기를 전담하는 넘인듯
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexShader;
	PixelShader pixelShader;
	PixelShader pixelShaderNoLight;
	ConstantBuffer<VSConstantBuffer> vsConstantBuffer;
	ConstantBuffer<PSLightConstantBuffer> psConstantBuffer;

	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	ComPtr<ID3D11RasterizerState> rasterizerState;
	ComPtr<ID3D11RasterizerState> rasterizerState_CULLFront;
	ComPtr<ID3D11BlendState> blendState;

	ComPtr<ID3D11SamplerState> samplerState;

	unique_ptr<DirectX::SpriteBatch> spriteBatch;
	unique_ptr<DirectX::SpriteFont> spriteFont;
};
