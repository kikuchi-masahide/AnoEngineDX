#include "DX12SwapChain.h"
#include "DX12Resource.h"
#include "DX12DescriptorHeap.h"
#include "DX12Pimple.h"
#include "../Log.h"

DX12SwapChain::DX12SwapChain(ComPtr<IDXGIFactory6> _factory, ComPtr<ID3D12CommandQueue> _cmdqueue, ComPtr<ID3D12Device> _device, HWND _hwnd, UINT _width, UINT _height)
{
	mBaseDesc.Width = _width;
	mBaseDesc.Height = _height;
	if (FAILED(
		_factory->CreateSwapChainForHwnd(
			_cmdqueue.Get(), _hwnd, &mBaseDesc, nullptr, nullptr,
			(IDXGISwapChain1**)mSwapChain.ReleaseAndGetAddressOf()
		)
	))
	{
		Log::OutputCritical("IDXGISwapChain1 Initialization failed");
		throw 0;
	}
#ifdef _DEBUG
	std::string msg("IDXGISwapChain1 Initialization successed");
	Log::OutputTrivial(msg);
#endif
	mDescHeap = boost::shared_ptr<DX12DescriptorHeap>(DBG_NEW DX12DescriptorHeap(DX12Config::DescriptorHeapType::RTV, DX12Config::DescriptorHeapShaderVisibility::NONE, 2,_device, L"SwapChain"));
	//ディスクリプタヒープにレンダーターゲットを作成しスワップチェーンと紐づけ
	for (UINT n = 0; n < 2; n++) {
		ComPtr<ID3D12Resource> _backbuffer;
		mBuffers[n] = boost::shared_ptr<DX12Resource>(DBG_NEW DX12Resource(mSwapChain, n));
		mBuffers[n]->CreateRenderTargetView(_device, mDescHeap, n);
	}
}

void DX12SwapChain::Filp()
{
	mSwapChain->Present(1, 0);
}

UINT DX12SwapChain::GetCurrentBackBufferIndex()
{
	return mSwapChain->GetCurrentBackBufferIndex();
}

void DX12SwapChain::ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> _list, float _r, float _g, float _b)
{
	auto bbid = mSwapChain->GetCurrentBackBufferIndex();
	auto handle = mDescHeap->GetCPUDescriptorHandle(bbid);
	//画面クリア
	float clearcolor[] = { _r,_g,_b,1.0f };
	_list->ClearRenderTargetView(handle, clearcolor, 0, nullptr);
}

void DX12SwapChain::OpenRenderTargets(ComPtr<ID3D12GraphicsCommandList> _list)
{
	auto bbid = mSwapChain->GetCurrentBackBufferIndex();
	auto handle = mDescHeap->GetCPUDescriptorHandle(bbid);
	//今から塗りつぶす方のバックバッファのリソースバリアの設定
	mBuffers[bbid]->SetResourceBarrier(_list, DX12Config::ResourceBarrierState::PRESENT, DX12Config::ResourceBarrierState::RENDER_TARGET);
	_list->OMSetRenderTargets(1, &handle, false, nullptr);
}

void DX12SwapChain::CloseRenderTarget(ComPtr<ID3D12GraphicsCommandList> _list)
{
	auto bbid = mSwapChain->GetCurrentBackBufferIndex();
	mBuffers[bbid]->SetResourceBarrier(_list, DX12Config::ResourceBarrierState::RENDER_TARGET, DX12Config::ResourceBarrierState::PRESENT);
}

DXGI_SWAP_CHAIN_DESC1 DX12SwapChain::mBaseDesc = {
	0,0,DXGI_FORMAT_R8G8B8A8_UNORM,false,1,0,DXGI_USAGE_BACK_BUFFER,2,
	DXGI_SCALING_STRETCH,DXGI_SWAP_EFFECT_FLIP_DISCARD,DXGI_ALPHA_MODE_UNSPECIFIED,
	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
};

void DX12Pimple::OpenRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain)
{
	_swapchain->OpenRenderTargets(mCmdList);
}

void DX12Pimple::CloseRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain)
{
	_swapchain->CloseRenderTarget(mCmdList);
}

void DX12Pimple::ClearRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain, float _r, float _g, float _b)
{
	_swapchain->ClearRenderTarget(mCmdList, _r, _g, _b);
}