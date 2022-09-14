//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Texture2D.h"

DX12::Texture2D::Texture2D(ComPtr<IDXGISwapChain> swapchain, int index, DXGI_FORMAT dxgi_format)
	:dxgi_format_(dxgi_format), width_(0), height_(0)
{
	auto res = swapchain->GetBuffer(index, IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		throw 0;
	}
}

DX12::Texture2D::Texture2D(ComPtr<ID3D12Device> device, UINT64 width, UINT height, DXGI_FORMAT dxgi_format, D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout, D3D12_RESOURCE_STATES state)
	:Resource(device, D3D12_HEAP_FLAG_NONE, heap_type,
		CD3DX12_RESOURCE_DESC::Tex2D(
			dxgi_format, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_NONE, texture_layout), state),
	dxgi_format_(dxgi_format), width_(width), height_(height)
{
}
