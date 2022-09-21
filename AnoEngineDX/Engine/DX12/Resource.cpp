//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Resource.h"

DX12::Resource::Resource()
{
}

DX12::Resource::Resource(ComPtr<ID3D12Device> device, D3D12_HEAP_FLAGS heap_flag, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_DESC resource_desc, D3D12_RESOURCE_STATES state)
{
	auto heapprop = CD3DX12_HEAP_PROPERTIES(heap_type);
	auto res = device->CreateCommittedResource(&heapprop, heap_flag, &resource_desc, state, nullptr, IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		throw 0;
	}
}

void DX12::Resource::SetDebugName(LPCWSTR debug_name)
{
	resource_->SetName(debug_name);
}

void* DX12::Resource::Map()
{
	void* map;
	resource_->Map(0, nullptr, &map);
	return map;
}

void DX12::Resource::Unmap()
{
	resource_->Unmap(0,nullptr);
}

bool DX12::Resource::IsValid() const
{
	return resource_;
}

D3D12_GPU_VIRTUAL_ADDRESS DX12::Resource::GetGPUVirtualAddress() const
{
	return resource_->GetGPUVirtualAddress();
}

ID3D12Resource* DX12::Resource::GetRawPtr() const
{
	return resource_.Get();
}
