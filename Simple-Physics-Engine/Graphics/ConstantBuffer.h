#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
#include "../Utils/ErrorLogger.h"
#include <DirectXMath.h>

struct VSConstantBuffer
{
	// 이거 16byte 단위로 뭐 패딩 넣어줘야하고 그렇거 있었는데.. 작동 이상하게 된다면 유튜브 "C++ DirectX 11 Engine Tutorial 26 - Constant Buffer Packing Alignment" 참고
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct PSConstantBuffer
{
	// 16byte
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength;
	// 16byte
	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicLightStrength;
	// 16byte
	DirectX::XMFLOAT3 dynamicLightPosition;
	float dynamicLightAttenA;
	float dynamicLightAttenB;
	float dynamicLightAttenC;
};

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext = nullptr;

public:
	ConstantBuffer() {}
	T data;
	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		ERROR_IF_FAILED(device->CreateBuffer(&desc, 0, buffer.GetAddressOf()), "컨스탠트 버퍼 생성에 실패했습니다");
	}
	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "컨스탠트 버퍼 매핑에 실패했습니다");
			return false;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->deviceContext->Unmap(buffer.Get(), 0);
		return true;
	}
};

#endif