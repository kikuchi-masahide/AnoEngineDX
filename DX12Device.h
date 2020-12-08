#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Device final:public boost::noncopyable {
public:
	DX12Device();
	//èâä˙âª
	void Initialize();
	ComPtr<ID3D12Device> GetDevice();
	void CleanUp();
private:
	ComPtr<ID3D12Device> mDevice;
};