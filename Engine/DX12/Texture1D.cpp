//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Texture1D.h"

DX12::Texture1D::Texture1D()
	:dxgi_format_(DXGI_FORMAT_UNKNOWN),width_(0)
{
}

DX12::Texture1D::Texture1D(ComPtr<ID3D12Device> device, UINT64 width, DXGI_FORMAT dxgi_format,
	D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout, D3D12_RESOURCE_STATES state)
	:Resource(device, D3D12_HEAP_FLAG_NONE,heap_type,
		CD3DX12_RESOURCE_DESC::Tex1D(dxgi_format,width,1,0,D3D12_RESOURCE_FLAG_NONE,texture_layout,0),state),
	width_(width),dxgi_format_(dxgi_format)
{
}

DXGI_FORMAT DX12::Texture1D::GetDXGIFormat() const
{
	return dxgi_format_;
}

UINT64 DX12::Texture1D::GetWidth() const
{
	return width_;
}
