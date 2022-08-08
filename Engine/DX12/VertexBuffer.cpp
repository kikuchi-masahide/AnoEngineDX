//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "VertexBuffer.h"

DX12::VertexBuffer::VertexBuffer(ComPtr<ID3D12Device> device, SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name)
	:size_per_vert_(size_per_vert),vertex_num_(vert_num)
{
	SIZE_T whole_size = size_per_vert_ * vertex_num_;
	//256ƒAƒ‰ƒCƒ“ƒƒ“ƒg
	whole_size = (whole_size + 0xff) & ~0xff;
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourcedesc = CD3DX12_RESOURCE_DESC::Buffer(whole_size);
	auto res = device->CreateCommittedResource(
		&heapprop, D3D12_HEAP_FLAG_NONE, &resourcedesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(resource_.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(res));
#ifdef _DEBUG
	resource_->SetName(debug_name);
#endif
}

void* DX12::VertexBuffer::Map()
{
	void* map;
	resource_->Map(0, nullptr, &map);
	return map;
}

void DX12::VertexBuffer::Unmap()
{
	resource_->Unmap(0, nullptr);
}

void DX12::VertexBuffer::SetVertexBuffer(ComPtr<ID3D12GraphicsCommandList> list, int slotid)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	vbView.BufferLocation = resource_->GetGPUVirtualAddress();
	vbView.SizeInBytes = size_per_vert_ * vertex_num_;
	vbView.StrideInBytes = size_per_vert_;
	list->IASetVertexBuffers(slotid, 1, &vbView);
}

D3D12_GPU_VIRTUAL_ADDRESS DX12::VertexBuffer::GetGPUVirtualAddress() const
{
	return resource_->GetGPUVirtualAddress();
}
