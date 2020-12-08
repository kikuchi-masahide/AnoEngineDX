#include "SwapChainManager.h"
#include "DX12Factory.h"
#include "DX12Device.h"
#include "DX12CmdQueue.h"
#include "DX12DescriptorHeap.h"
#include "DX12CmdList.h"

void SwapChainManager::Initialize()
{
}

void SwapChainManager::CleanUp()
{
}

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
		ID3D12Resource* _backbuffer;
		if (FAILED(
			mSwapChains.back()->GetBuffer(n, IID_PPV_ARGS(&_backbuffer))
		)) {
			Log::OutputCritical("mapping of backbuffers and RTV failed");
			throw 0;
		}
		auto handle = _descheap->GetCPUDescriptorHandle(n);
		dev->CreateRenderTargetView(_backbuffer, nullptr, handle);
	}
	mDescHeaps.push_back(_descheap);
	return (unsigned int)mSwapChains.size() - 1;
}

void SwapChainManager::FlipAll()
{
	for (auto p : mSwapChains)
	{
		p->Present(1, 0);
	}
}

void SwapChainManager::SetAndClearRenderTarget(unsigned int _id, DX12CmdList* _list, float _r, float _g, float _b)
{
	//バックバッファのインデックス
	auto bbidx = mSwapChains[_id]->GetCurrentBackBufferIndex();
	//ハンドルを取る
	auto handle = mDescHeaps[_id]->GetCPUDescriptorHandle(bbidx);
	auto list = _list->GetCmdList();
	//設定
	list->OMSetRenderTargets(1, &handle, false, nullptr);
	//画面クリア
	float clearcolor[] = { _r,_g,_b,1.0f };
	list->ClearRenderTargetView(handle,clearcolor,0,nullptr);
}

DXGI_SWAP_CHAIN_DESC1 SwapChainManager::mBaseDesc = {
	0,0,DXGI_FORMAT_R8G8B8A8_UNORM,false,1,0,DXGI_USAGE_BACK_BUFFER,2,
	DXGI_SCALING_STRETCH,DXGI_SWAP_EFFECT_FLIP_DISCARD,DXGI_ALPHA_MODE_UNSPECIFIED,
	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
};