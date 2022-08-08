//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "SwapChain.h"
#include "DescriptorHeap.h"

#include "Log.h"

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
	D3D12_DESCRIPTOR_HEAP_DESC heapdesc = {
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,2,D3D12_DESCRIPTOR_HEAP_FLAG_NONE,0
	};
	if (FAILED(device->CreateDescriptorHeap(
		&heapdesc, IID_PPV_ARGS(desc_heap_.ReleaseAndGetAddressOf())
	))) {
		Log::OutputCritical("Initialization of ID3D12DescriptorHeap in DX12::SwapChain failed");
		throw 0;
	}
#ifdef _DEBUG
	desc_heap_->SetName(L"DX12::SwapChain::desc_heap_");
#endif
	D3D12_RENDER_TARGET_VIEW_DESC rtvdesc = {};
	rtvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	auto handle = desc_heap_->GetCPUDescriptorHandleForHeapStart();
	desc_handle_incr_size_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (int n = 0; n < 2; n++) {
		if (FAILED(swapchain_->GetBuffer(n, IID_PPV_ARGS(buffer_[n].ReleaseAndGetAddressOf())))) {
			Log::OutputCritical("Mapping of backbuffer failed");
			throw 0;
		}
		device->CreateRenderTargetView(buffer_[n].Get(), &rtvdesc, handle);
		handle.ptr += desc_handle_incr_size_;
	}

}

DX12::SwapChain::~SwapChain()
{
}

UINT DX12::SwapChain::GetCurrentBackBufferIndex()
{
	return swapchain_->GetCurrentBackBufferIndex();
}

void DX12::SwapChain::SetResourceBarrier(ComPtr<ID3D12GraphicsCommandList> cmdlist, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	static D3D12_RESOURCE_BARRIER barrier = {
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,D3D12_RESOURCE_BARRIER_FLAG_NONE ,{}
	};
	auto bbid = GetCurrentBackBufferIndex();
	barrier.Transition = {
		buffer_[bbid].Get(),D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,before,after
	};
	cmdlist->ResourceBarrier(1, &barrier);
}

void DX12::SwapChain::ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> list, float r, float g, float b)
{
	auto bbid = swapchain_->GetCurrentBackBufferIndex();
	auto handle = desc_heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += desc_handle_incr_size_ * bbid;
	float clearcolor[] = { r,g,b,1.0f };
	list->ClearRenderTargetView(handle, clearcolor, 0, nullptr);
}

void DX12::SwapChain::SetRenderTarget(ComPtr<ID3D12GraphicsCommandList> cmdlist)
{
	auto bbid = GetCurrentBackBufferIndex();
	auto handle = desc_heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += desc_handle_incr_size_ * bbid;
	cmdlist->OMSetRenderTargets(1, &handle, false, nullptr);
}

void DX12::SwapChain::SetRenderTarget(ComPtr<ID3D12GraphicsCommandList> cmdlist, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	auto bbid = GetCurrentBackBufferIndex();
	auto handle = desc_heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += desc_handle_incr_size_ * bbid;
	auto rtv_handle = desc_heap->GetCPUDescriptorHandle(index);
	cmdlist->OMSetRenderTargets(1, &handle,false,&rtv_handle);
}

void DX12::SwapChain::Flip()
{
	swapchain_->Present(1, 0);
}
