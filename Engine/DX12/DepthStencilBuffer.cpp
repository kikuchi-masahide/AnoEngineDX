//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "DepthStencilBuffer.h"

DX12::DepthStencilBuffer::DepthStencilBuffer(ComPtr<ID3D12Device> device, UINT64 width, UINT64 height, ResourceHeapType heap_type)
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	CD3DX12_HEAP_PROPERTIES heapprop(static_cast<D3D12_HEAP_TYPE>(heap_type));
	D3D12_CLEAR_VALUE depthclear = {};
	depthclear.DepthStencil.Depth = 1.0f;
	depthclear.Format = DXGI_FORMAT_D32_FLOAT;
	auto result = device->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthclear, IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

ID3D12Resource* DX12::DepthStencilBuffer::GetRawPtr() const
{
	return resource_.Get();
}
