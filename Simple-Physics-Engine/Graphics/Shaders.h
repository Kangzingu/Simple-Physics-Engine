#pragma once
#include "../Utils/ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;// DX 11���� ���� ���Ŀ� �´� ���̴�, Blob�� ������ ���� ����
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;// �ӽ÷�? raw��? ���̴� ������ ��� ����
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;// DX 11���� ���� ���Ŀ� �´� ���̴�, Blob�� ������ ���� ����
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;// �ӽ÷�? raw��? ���̴� ������ ��� ����
};