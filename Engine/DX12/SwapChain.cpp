//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "SwapChain.h"

#include "Texture2D.h"
#include "Log.h"

DX12::SwapChain::SwapChain()
{
}

DX12::SwapChain::SwapChain(ComPtr<IDXGIFactory6> factory, ComPtr<ID3D12CommandQueue> cmdqueue,
	ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height)
{
	DXGI_SWAP_CHAIN_DESC1 basedesc = {
		width,height,DXGI_FORMAT_R8G8B8A8_UNORM,false,1,0,DXGI_USAGE_BACK_BUFFER,2,
		DXGI_SCALING_STRETCH,DXGI_SWAP_EFFECT_FLIP_DISCARD,DXGI_ALPHA_MODE_UNSPECIFIED,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};
	if (FAILED(factory->CreateSwapChainForHwnd(
		cmdqueue.Get(), hwnd, &basedesc, nullptr, nullptr,
		(IDXGISwapChain1**)swapchain_.ReleaseAndGetAddressOf()
	))) {
		Log::OutputCritical("Initialization of IDXGISwapCHain1 failed");
		throw 0;
	}
	desc_heap_ = DescriptorHeap(device, 2, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	for (int n = 0; n < 2; n++) {
		//このbackbuffer_[n]からRTVを作るので、formatはUNORM_SRGBにしておく
		backbuffer_[n] = Texture2D(swapchain_, n, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		desc_heap_.CreateRenderTargetView(device, backbuffer_[n], n);
	}
}

void DX12::SwapChain::SetDebugName(LPCWSTR debug_name)
{
#ifdef _DEBUG
	backbuffer_[0].SetDebugName(debug_name);
	backbuffer_[1].SetDebugName(debug_name);
	desc_heap_.SetDebugName(debug_name);
#endif
}

void DX12::SwapChain::Flip()
{
	swapchain_->Present(1, 0);
}

bool DX12::SwapChain::IsValid() const
{
	return swapchain_;
}

UINT DX12::SwapChain::GetCurrentBackBufferIndex() const
{
	return swapchain_->GetCurrentBackBufferIndex();
}

DX12::Texture2D DX12::SwapChain::GetCurrentBackBuffer() const
{
	return backbuffer_[GetCurrentBackBufferIndex()];
}

const DX12::DescriptorHeap DX12::SwapChain::GetDescriptorHeap() const
{
	return desc_heap_;
}

