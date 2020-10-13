#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

/// <summary>
/// スワップチェーンのマネージャ
/// </summary>
class SwapChainManager : public boost::noncopyable{
private:
	std::vector<ComPtr<IDXGISwapChain1>> mSwapChains;
	//スワップチェーンの設定のうち共通するもの
	static DXGI_SWAP_CHAIN_DESC1 mBaseDesc;
public:
	/// <summary>
	/// スワップチェーンを加える
	/// </summary>
	/// <returns>キー</returns>
	unsigned int AddSwapChain(
		ComPtr<IDXGIFactory6>& _factory, ComPtr<ID3D12CommandQueue>& _cmdqueue,
		HWND _hwnd, UINT _width, UINT _height);
};