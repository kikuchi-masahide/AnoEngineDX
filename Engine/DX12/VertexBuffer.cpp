//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "VertexBuffer.h"

DX12::VertexBuffer::VertexBuffer()
	:size_per_vert_(0),vertex_num_(0)
{
}

DX12::VertexBuffer::VertexBuffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state, SIZE_T size_per_vert, int vert_num)
	:Buffer(device,heap_type,size_per_vert*vert_num,state),size_per_vert_(size_per_vert),vertex_num_(vert_num)
{
}

SIZE_T DX12::VertexBuffer::GetSizePerVert() const
{
	return size_per_vert_;
}

int DX12::VertexBuffer::GetVertexNum() const
{
	return vertex_num_;
}
