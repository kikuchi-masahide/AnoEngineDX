//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ShaderResource.h"

DX12::ShaderResource::ShaderResource(ComPtr<ID3D12Device> device, DX12::ResourceHeapType heap_type, UINT64 width, UINT64 height, float r, float g, float b, float alpha, LPCWSTR debug_name)
	:format_(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB),width_(width),height_(height)
{
	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Width = width_;
	resdesc.Height = height_;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = format_;
	resdesc.SampleDesc.Count = 1;
	resdesc.SampleDesc.Quality = 0;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	auto heapprop = CD3DX12_HEAP_PROPERTIES(static_cast<D3D12_HEAP_TYPE>(heap_type));
	float clsClr[4] = { r,g,b,alpha };
	auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clsClr);
	auto result = device->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	resource_->SetName(debug_name);
#endif
}

DX12::ShaderResource::ShaderResource(ComPtr<ID3D12Device> device, DX12::ResourceHeapType heap_type, DXGI_FORMAT format, D3D12_RESOURCE_DIMENSION dimension, UINT64 width, UINT64 height, D3D12_TEXTURE_LAYOUT layout, D3D12_RESOURCE_FLAGS resource_flag,D3D12_HEAP_FLAGS heap_flag,ResourceBarrierState resource_state,LPCWSTR debug_name)
	:format_(format),width_(width),height_(height)
{
	CD3DX12_HEAP_PROPERTIES heapprop(static_cast<D3D12_HEAP_TYPE>(heap_type));
	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Format = format;
	resdesc.Dimension = dimension;
	resdesc.Width = width;
	resdesc.Height = height;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Layout = layout;
	resdesc.Flags = resource_flag;
	resdesc.SampleDesc.Count = 1;
	resdesc.SampleDesc.Quality = 0;
	D3D12_RESOURCE_STATES st0 = D3D12_RESOURCE_STATE_GENERIC_READ;
	D3D12_RESOURCE_STATES st = static_cast<D3D12_RESOURCE_STATES>(resource_state);
	auto result = device->CreateCommittedResource(&heapprop, heap_flag, &resdesc,
		st, nullptr,
		IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	resource_->SetName(debug_name);
#endif
}

void* DX12::ShaderResource::Map()
{
	void* map;
	resource_->Map(0, nullptr, &map);
	return map;
}

void DX12::ShaderResource::Unmap()
{
	resource_->Unmap(0, nullptr);
}

DXGI_FORMAT DX12::ShaderResource::GetDXGIFormat() const
{
	return format_;
}

UINT64 DX12::ShaderResource::GetWidth() const
{
	return width_;
}

UINT64 DX12::ShaderResource::GetHeight() const
{
	return height_;
}

ID3D12Resource* DX12::ShaderResource::GetRawPtr() const
{
	return resource_.Get();
}

D3D12_RESOURCE_DESC DX12::ShaderResource::GetDesc() const
{
	return resource_->GetDesc();
}
