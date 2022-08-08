//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "IndexBuffer.h"

DX12::IndexBuffer::IndexBuffer(ComPtr<ID3D12Device> device, int vertex_num, LPCWSTR debug_name)
	:vertex_num_(vertex_num)
{
	SIZE_T whole_size = sizeof(unsigned int) * vertex_num_;
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

unsigned int* DX12::IndexBuffer::Map()
{
	void* map_v;
	resource_->Map(0, nullptr, &map_v);
	return static_cast<unsigned int*>(map_v);
}

void DX12::IndexBuffer::Unmap()
{
	resource_->Unmap(0, nullptr);
}

void DX12::IndexBuffer::SetIndexBuffer(ComPtr<ID3D12GraphicsCommandList> list)
{
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ibView.BufferLocation = resource_->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeof(unsigned int) * vertex_num_;
	list->IASetIndexBuffer(&ibView);
}

D3D12_GPU_VIRTUAL_ADDRESS DX12::IndexBuffer::GetGPUVirtualAddress() const
{
	return resource_->GetGPUVirtualAddress();
}
