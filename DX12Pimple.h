#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")

#include "SwapChainManager.h"

using namespace Microsoft::WRL;

class DX12Pimple:public boost::noncopyable
{
private:
	ComPtr<ID3D12Device> mDevice;
	ComPtr<IDXGIFactory6> mFactory;
	void EnableDebugLayer();
	ComPtr<ID3D12GraphicsCommandList> mCmdList;
	ComPtr<ID3D12CommandAllocator> mCmdAllocator;
	ComPtr<ID3D12CommandQueue> mCmdQueue;
	//コマンドキューの初期化
	void CreateCommandQueue();
	//スワップチェーンたち
	SwapChainManager mSwapChainManager;
public:
	DX12Pimple();
	void Initialize();
	void CleanUp();
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
};