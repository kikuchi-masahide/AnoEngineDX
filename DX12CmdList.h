#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Device;
class DX12CmdAllocator;

class DX12CmdList final:public boost::noncopyable{
public:
	DX12CmdList();
	void Initialize(DX12Device* _device, DX12CmdAllocator* _cmdalloc);
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	void CleanUp();
private:
	ComPtr<ID3D12GraphicsCommandList> mCmdList;
};