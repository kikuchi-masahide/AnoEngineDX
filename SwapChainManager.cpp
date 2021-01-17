#include "SwapChainManager.h"
#include "DX12Factory.h"
#include "DX12Device.h"
#include "DX12CmdQueue.h"
#include "DX12DescriptorHeap.h"
#include "DX12CmdList.h"

void SwapChainManager::ClearRenderTarget(DX12CmdList* _list, float _r, float _g, float _b)
{
	auto list = _list->GetCmdList();
	//バックバッファのインデックス
	auto bbidx = mSwapChains[mCurrentBBid]->GetCurrentBackBufferIndex();
	//ハンドルを取る
	auto handle = mDescHeaps[mCurrentBBid]->GetCPUDescriptorHandle(bbidx);
	//画面クリア
	float clearcolor[] = { _r,_g,_b,1.0f };
	list->ClearRenderTargetView(handle, clearcolor, 0, nullptr);
}

void SwapChainManager::CloseRenderTarget(DX12CmdList* _list)
{
	auto list = _list->GetCmdList();
	//バックバッファのインデックス
	auto bbidx = mSwapChains[mCurrentBBid]->GetCurrentBackBufferIndex();
	//今から塗りつぶす方のバックバッファのリソースバリアの設定
	mResourceBarrierDesc.Transition.pResource = mBackBuffers[bbidx][mCurrentBBid].Get();
	mResourceBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mResourceBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	list->ResourceBarrier(1, &mResourceBarrierDesc);
}

void SwapChainManager::Initialize()
{
	//リソースバリア
	mResourceBarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	mResourceBarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	mResourceBarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
}

void SwapChainManager::CleanUp()
{}

unsigned int SwapChainManager::AddSwapChain(
	DX12Factory* _factory, DX12CmdQueue* _cmdqueue, DX12Device* _device, HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap)
{
	mBaseDesc.Width = _width;
	mBaseDesc.Height = _height;
	mSwapChains.emplace_back(nullptr);
	if (FAILED(
		_factory->GetFactory()->CreateSwapChainForHwnd(
			_cmdqueue->GetCmdQueue().Get(), _hwnd, &mBaseDesc, nullptr, nullptr,
			(IDXGISwapChain1**)mSwapChains.back().ReleaseAndGetAddressOf()
		)
	))
	{
		Log::OutputCritical("IDXGISwapChain1 Initialization failed");
		throw 0;
	}
#ifdef _DEBUG
	std::string msg("IDXGISwapChain1 Initialization successed with ID ");
	msg += std::to_string((unsigned int)mSwapChains.size() - 1);
	Log::OutputTrivial(msg);
#endif
	//ディスクリプタヒープにレンダーターゲットを作成しスワップチェーンと紐づけ
	auto dev = _device->GetDevice();
	for (int n = 0; n < 2; n++) {
		ComPtr<ID3D12Resource> _backbuffer;
		if (FAILED(
			mSwapChains.back()->GetBuffer(n, IID_PPV_ARGS(_backbuffer.ReleaseAndGetAddressOf()))
		)) {
			Log::OutputCritical("mapping of backbuffers and RTV failed");
			throw 0;
		}
		auto handle = _descheap->GetCPUDescriptorHandle(n);
		dev->CreateRenderTargetView(_backbuffer.Get(), nullptr, handle);
		mBackBuffers[n].push_back(_backbuffer);
	}
	mDescHeaps.push_back(_descheap);
	return (unsigned int)mSwapChains.size() - 1;
}

void SwapChainManager::FlipAll(DX12Device* _dev)
{
	auto dev = _dev->GetDevice();
	for (auto p : mSwapChains)
	{
		p->Present(1, 0);
		auto result = dev->GetDeviceRemovedReason();
		if (FAILED(result)) {
			throw 0;
		}
	}
}

void SwapChainManager::OpenRenderTarget(unsigned int _id, DX12CmdList* _list)
{
	auto list = _list->GetCmdList();
	mCurrentBBid = _id;
	//バックバッファのインデックス
	auto bbidx = mSwapChains[_id]->GetCurrentBackBufferIndex();
	//今から塗りつぶす方のバックバッファのリソースバリアの設定
	mResourceBarrierDesc.Transition.pResource = mBackBuffers[bbidx][_id].Get();
	mResourceBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	mResourceBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	list->ResourceBarrier(1, &mResourceBarrierDesc);
	//ハンドルを取る
	auto handle = mDescHeaps[_id]->GetCPUDescriptorHandle(bbidx);
	//設定
	list->OMSetRenderTargets(1, &handle, false, nullptr);
}

DXGI_SWAP_CHAIN_DESC1 SwapChainManager::mBaseDesc = {
	0,0,DXGI_FORMAT_R8G8B8A8_UNORM,false,1,0,DXGI_USAGE_BACK_BUFFER,2,
	DXGI_SCALING_STRETCH,DXGI_SWAP_EFFECT_FLIP_DISCARD,DXGI_ALPHA_MODE_UNSPECIFIED,
	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
};

D3D12_RESOURCE_BARRIER SwapChainManager::mResourceBarrierDesc = {};