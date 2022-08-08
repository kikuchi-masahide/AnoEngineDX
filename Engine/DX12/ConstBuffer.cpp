//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ConstBuffer.h"

DX12::ConstBuffer::ConstBuffer(ComPtr<ID3D12Device> device, SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name)
	:whole_256ed_size_((size + 0xff) & ~0xff)
{
	auto heapprop = CD3DX12_HEAP_PROPERTIES(static_cast<D3D12_HEAP_TYPE>(heap_type));
	auto resourcedesc = CD3DX12_RESOURCE_DESC::Buffer(whole_256ed_size_);
	auto res = device->CreateCommittedResource(
		&heapprop, D3D12_HEAP_FLAG_NONE, &resourcedesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(res));
#ifdef _DEBUG
	resource_->SetName(debug_name);
#endif
}

void* DX12::ConstBuffer::Map()
{
	void* map;
	resource_->Map(0, nullptr, &map);
	return map;
}

void DX12::ConstBuffer::Unmap()
{
	resource_->Unmap(0,nullptr);
}

D3D12_GPU_VIRTUAL_ADDRESS DX12::ConstBuffer::GetGPUVirtualAddress()
{
	return resource_->GetGPUVirtualAddress();
}
