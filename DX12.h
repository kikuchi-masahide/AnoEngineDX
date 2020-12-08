#pragma once

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")

class DX12Factory;
class DX12Device;
class DX12CmdAllocator;
class DX12CmdList;
class DX12CmdQueue;
class SwapChainManager;

/// <summary>
/// Gameに含まれる，DirectX12をラップしたクラス
/// </summary>
class DX12:public boost::noncopyable
{
private:
	DX12Factory* mFactory;
	DX12Device* mDevice;
	DX12CmdAllocator* mCmdAllocator;
	DX12CmdList* mCmdList;
	DX12CmdQueue* mCmdQueue;
	SwapChainManager* mSwapChainManager;
public:
	DX12();
	void Initialize();
	void CleanUp();
	/// <summary>
	/// スワップチェーンを作る
	/// </summary>
	/// <returns>スワップチェーンのキー</returns>
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
};