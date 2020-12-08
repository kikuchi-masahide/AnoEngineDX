#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Device :public boost::noncopyable {
public:
	DX12Device();
	//������
	void Initialize();
	ComPtr<ID3D12Device> GetDevice();
private:
	ComPtr<ID3D12Device> mDevice;
};