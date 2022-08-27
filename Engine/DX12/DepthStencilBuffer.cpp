//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "DepthStencilBuffer.h"

DX12::DepthStencilBuffer::DepthStencilBuffer(ComPtr<ID3D12Device> device, UINT64 width, UINT height, D3D12_HEAP_TYPE heap_type)
	:Resource(device,D3D12_HEAP_FLAG_NONE,heap_type,CD3DX12_RESOURCE_DESC::Tex2D(
	DXGI_FORMAT_D32_FLOAT,width,height,1,0,1,0,D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	),D3D12_RESOURCE_STATE_DEPTH_WRITE)
{
}
