#pragma once

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

#include "DX12Device.h"
#include "DX12Factory.h"
#include "DX12CmdAllocator.h"
#include "DX12CmdList.h"
#include "DX12CmdQueue.h"
#include "SwapChainManager.h"

using namespace Microsoft::WRL;

class DX12Pimple:public boost::noncopyable
{
private:
	DX12Device mDevice;
	DX12Factory mFactory;
	void EnableDebugLayer();
	DX12CmdList mCmdList;
	DX12CmdAllocator mCmdAllocator;
	DX12CmdQueue mCmdQueue;
	//スワップチェーンたち
	SwapChainManager mSwapChainManager;
public:
	DX12Pimple();
	void Initialize();
	void CleanUp();
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
};