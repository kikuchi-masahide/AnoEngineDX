//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "IndexBuffer.h"

DX12::IndexBuffer::IndexBuffer()
	:vertex_num_(0)
{
}

DX12::IndexBuffer::IndexBuffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state, int vert_num)
	:Buffer(device,heap_type,sizeof(unsigned int)*vert_num,state),vertex_num_(vert_num)
{
}

int DX12::IndexBuffer::GetVertexNum() const
{
	return vertex_num_;
}
