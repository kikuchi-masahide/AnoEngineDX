#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Factory;
class DX12CmdQueue;
class DX12Device;
class DX12DescriptorHeap;
class DX12CmdList;

/// <summary>
/// スワップチェーンのマネージャ
/// </summary>
class SwapChainManager final : public boost::noncopyable {
private:
	std::vector<ComPtr<IDXGISwapChain4>> mSwapChains;
	//各スワップチェーンに対するビューを持つディスクリプタヒープ
	//(1つのディスクリプタヒープで1つのスワップチェーンを管理すると確約する)
	std::vector<boost::shared_ptr<DX12DescriptorHeap>> mDescHeaps;
	//リソース(mBackBuffers[BackBuffer何枚目か][id])
	std::vector<ComPtr<ID3D12Resource>> mBackBuffers[2];
	//スワップチェーンの設定のうち共通するもの
	static DXGI_SWAP_CHAIN_DESC1 mBaseDesc;
	//リソースバリアの設定を行う構造体
	static D3D12_RESOURCE_BARRIER mResourceBarrierDesc;
public:
	void Initialize();
	void CleanUp();
	/// <summary>
	/// スワップチェーンを加える
	/// </summary>
	/// <returns>キー</returns>
	unsigned int AddSwapChain(
		DX12Factory* _factory, DX12CmdQueue* _cmdqueue, DX12Device* _device, HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap);
	/// <summary>
	/// 全スワップチェーンをフリップ
	/// </summary>
	void FlipAll();
	/// <summary>
	/// _idの指すスワップチェーンをレンダーターゲットに指定しリソースバリアを設定．背景色も同時に設定する
	/// </summary>
	void SetAndClearRenderTarget(unsigned int _id,DX12CmdList* _list,float _r,float _g,float _b);
};