#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Device;

class DX12CmdAllocator final :public boost::noncopyable {
public:
	DX12CmdAllocator();
	void Initialize(DX12Device* _device);
	ComPtr<ID3D12CommandAllocator> GetCmdAllocator();
	void CleanUp();
private:
	ComPtr<ID3D12CommandAllocator> mCmdAllocator;
};