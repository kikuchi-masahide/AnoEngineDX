//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "DescriptorHeap.h"

#include "ShaderResource.h"
#include "ConstBuffer.h"
#include "DepthStencilBuffer.h"

DX12::DescriptorHeap::DescriptorHeap(ComPtr<ID3D12Device> device, int capacity,
	D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS vis)
	:capacity_(capacity),heap_type_(type),shader_visibility_(vis)
{
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc_str;
	heap_desc_str.Type = heap_type_;
	heap_desc_str.NumDescriptors = capacity_;
	heap_desc_str.Flags = vis;
	heap_desc_str.NodeMask = 0;
	auto res = device->CreateDescriptorHeap(&heap_desc_str,IID_PPV_ARGS(desc_heap_.ReleaseAndGetAddressOf()));
	descriptor_handle_size_ = device->GetDescriptorHandleIncrementSize(heap_type_);
}

void DX12::DescriptorHeap::SetDebugName(LPCWSTR debug_name)
{
#ifdef _DEBUG
	desc_heap_->SetName(debug_name);
#endif
}

void DX12::DescriptorHeap::CreateRenderTargetView(ComPtr<ID3D12Device> device,
	std::shared_ptr<ShaderResource> shader_resource, int index)
{
	assert(heap_type_ == D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_RENDER_TARGET_VIEW_DESC rtvdesc = {};
	rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvdesc.Format = shader_resource->dxgi_format_;
	auto handle = GetCPUDescriptorHandle(index);
	device->CreateRenderTargetView(shader_resource->GetRawPtr(), &rtvdesc, handle);
}

void DX12::DescriptorHeap::CreateConstBufferView(ComPtr<ID3D12Device> device,
	std::shared_ptr<ConstBuffer> const_buff, int index)
{
	assert(heap_type_ == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = const_buff->GetGPUVirtualAddress();
	desc.SizeInBytes = const_buff->size_;
	auto cpuhandle = GetCPUDescriptorHandle(index);
	device->CreateConstantBufferView(&desc, cpuhandle);
}

void DX12::DescriptorHeap::CreateShaderResourceView(ComPtr<ID3D12Device> device,
	std::shared_ptr<ShaderResource> shader_resource, int index)
{
	assert(heap_type_ == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	auto handle = GetCPUDescriptorHandle(index);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvdesc = {};
	srvdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Format = static_cast<DXGI_FORMAT>(shader_resource->dxgi_format_);
	srvdesc.Texture2D.MipLevels = 1;
	srvdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	device->CreateShaderResourceView(shader_resource->GetRawPtr(), &srvdesc, handle);
}

void DX12::DescriptorHeap::CreateDepthStencilBufferView(ComPtr<ID3D12Device> device,
	std::shared_ptr<DepthStencilBuffer> buffer, int index)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_DSV_FLAG_NONE;
	device->CreateDepthStencilView(buffer->GetRawPtr(), &desc, GetCPUDescriptorHandle(index));
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12::DescriptorHeap::GetCPUDescriptorHandle(int index) const
{
	auto handle = desc_heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += descriptor_handle_size_ * index;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE DX12::DescriptorHeap::GetGPUDescriptorHandle(int index) const
{
	auto handle = desc_heap_->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += descriptor_handle_size_ * index;
	return handle;
}

ID3D12DescriptorHeap* DX12::DescriptorHeap::GetRawPtr() const
{
	return desc_heap_.Get();
}
