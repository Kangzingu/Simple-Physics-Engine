#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Camera3D.h"
#include "../Utils/Timer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "RenderableGameObject.h"
#include "Light.h"
#include "Camera2D.h"
#include "Sprite.h"

class Renderer
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void Render();

	Camera3D camera3D;

	RenderableGameObject gameObject;
	RenderableGameObject gameObject2;
	
	std::vector<RenderableGameObject> gameObjects;
	void UpdatePhysics();

	Light light;

private:
	void InitializeDirectX(HWND hwnd);
	void InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;// 그리기를 전담하는 넘인듯
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexshader;
	PixelShader pixelshader;
	PixelShader pixelshader_nolight;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_light> cb_ps_light;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CULLFront;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	int windowWidth = 0;
	int windowHeight = 0;

	float deltaTime = 0;

	Timer timer;
	Timer sceneTimer;
};