//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Buffer.h"

DX12::Buffer::Buffer()
	:size_(0)
{
}

DX12::Buffer::Buffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, SIZE_T size,
	D3D12_RESOURCE_STATES state)
	:Resource(device, D3D12_HEAP_FLAG_NONE, heap_type, CD3DX12_RESOURCE_DESC::Buffer((size + 0xff) & ~0xff), state),
	size_((size + 0xff) & ~0xff)
{
}

SIZE_T DX12::Buffer::GetSize() const
{
	return size_;
}
