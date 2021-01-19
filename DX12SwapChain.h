#pragma once
#include "DX12CommonInclude.h"

class DX12Resource;
class DX12DescriptorHeap;

class DX12SwapChain {
public:
	DX12SwapChain(ComPtr<IDXGIFactory6> _factory, ComPtr<ID3D12CommandQueue> _cmdqueue, ComPtr<ID3D12Device> _device, HWND _hwnd, UINT _width, UINT _height);
	/// <summary>
	/// フリップ
	/// </summary>
	void Filp();
	UINT GetCurrentBackBufferIndex();
	void ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> _list, float _r, float _g, float _b);
	/// <summary>
	/// _listからOMSetRenderTargetsを行う(リソースバリアも張る)
	/// </summary>
	void OpenRenderTargets(ComPtr<ID3D12GraphicsCommandList> _list);
	/// <summary>
	/// リソースバリアを設定しレンダーターゲットを閉じる
	/// </summary>
	void CloseRenderTarget(ComPtr<ID3D12GraphicsCommandList> _list);
private:
	ComPtr<IDXGISwapChain4> mSwapChain;
	//リソース(mBackBuffers[BackBuffer何枚目か][id])
	boost::shared_ptr<DX12Resource> mBuffers[2];
	//各スワップチェーンに対するビューを持つディスクリプタヒープ
	//(1つのディスクリプタヒープで1つのスワップチェーンを管理すると確約する)
	boost::shared_ptr<DX12DescriptorHeap> mDescHeap;
	//スワップチェーンの設定のうち共通するもの
	static DXGI_SWAP_CHAIN_DESC1 mBaseDesc;
};