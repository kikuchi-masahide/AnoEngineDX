//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "SwapChain.h"

#include "ShaderResource.h"
#include "DescriptorHeap.h"
#include "Log.h"

DX12::SwapChain::SwapChain(ComPtr<IDXGIFactory6> factory, ComPtr<ID3D12CommandQueue> cmdqueue,
	ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height)
{
	//DXGI_SWAP_CHAIN_DESC1 basedesc;
	//basedesc.Width = width;
	//basedesc.Height = height;
	//basedesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//basedesc.Stereo = false;
	//basedesc.SampleDesc.Count = 1;
	//basedesc.SampleDesc.Quality = 0;
	//basedesc.BufferCount = DXGI_USAGE_BACK_BUFFER;
	//basedesc.BufferCount = 2;
	//basedesc.Scaling = DXGI_SCALING_STRETCH;
	//basedesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//basedesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	//basedesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
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
	desc_heap_ = std::make_shared<DescriptorHeap>(device, 2,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	for (int n = 0; n < 2; n++) {
		//‚±‚Ìbackbuffer_[n]‚©‚çRTV‚ðì‚é‚Ì‚ÅAformat‚ÍUNORM_SRGB‚É‚µ‚Ä‚¨‚­
		backbuffer_[n] = std::make_shared<ShaderResource>(swapchain_, n, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		desc_heap_->CreateRenderTargetView(device, backbuffer_[n], n);
	}
}

void DX12::SwapChain::SetDebugName(LPCWSTR debug_name)
{
#ifdef _DEBUG
	backbuffer_[0]->SetDebugName(debug_name);
	backbuffer_[1]->SetDebugName(debug_name);
	desc_heap_->SetDebugName(debug_name);
#endif
}

void DX12::SwapChain::Flip()
{
	swapchain_->Present(1, 0);
}

UINT DX12::SwapChain::GetCurrentBackBufferIndex() const
{
	return swapchain_->GetCurrentBackBufferIndex();
}

std::shared_ptr<DX12::ShaderResource> DX12::SwapChain::GetCurrentBackBuffer() const
{
	return backbuffer_[GetCurrentBackBufferIndex()];
}

std::shared_ptr<DX12::DescriptorHeap> DX12::SwapChain::GetDescriptorHeap() const
{
	return desc_heap_;
}

