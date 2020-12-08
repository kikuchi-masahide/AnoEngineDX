#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Factory :public boost::noncopyable
{
public:
	DX12Factory();
	void Initialize();
	ComPtr<IDXGIFactory6> GetFactory();
private:
	ComPtr<IDXGIFactory6> mFactory;
};