#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Device;

class DX12CmdQueue final:public boost::noncopyable{
public:
	DX12CmdQueue();
	void Initialize(DX12Device* _device);
	ComPtr<ID3D12CommandQueue> GetCmdQueue();
	void CleanUp();
private:
	ComPtr<ID3D12CommandQueue> mCmdQueue;
};