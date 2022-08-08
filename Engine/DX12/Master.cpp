//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Master.h"

#include "ConfigEnums.h"
#include "Master_pimple.h"

void DX12::Master::Initialize() {
	pimple_ = DBG_NEW Master_pimple;
	try {
		pimple_->Initialize();
	}
	catch (...) {
		delete pimple_;
		throw;
	}
}

void DX12::Master::ProcessCommands()
{
	pimple_->ProcessCommands();
}

std::shared_ptr<DX12::ShaderObject> DX12::Master::LoadShader(LPCWSTR filename, DX12::ShaderType shader_type)
{
	return pimple_->LoadShader(filename, shader_type);
}

std::shared_ptr<DX12::SwapChain> DX12::Master::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	return pimple_->CreateSwapChain(hwnd, width, height);
}

std::shared_ptr<DX12::VertexBuffer> DX12::Master::CreateVertexBuffer(SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name)
{
	return pimple_->CreateVertexBuffer(size_per_vert, vert_num, debug_name);
}

std::shared_ptr<DX12::IndexBuffer> DX12::Master::CreateIndexBuffer(int vert_num, LPCWSTR debug_name)
{
	return pimple_->CreateIndexBuffer(vert_num, debug_name);
}

std::shared_ptr<DX12::DescriptorHeap> DX12::Master::CreateDescriptorHeap(int capacity, DescriptorHeapType type, DescriptorHeapShaderVisibility vis, LPCWSTR debug_name)
{
	return pimple_->CreateDescriptorHeap(capacity, type, vis, debug_name);
}

std::shared_ptr<DX12::ConstBuffer> DX12::Master::CreateConstBuffer(SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name)
{
	return pimple_->CreateConstBuffer(size, heap_type, debug_name);
}

std::shared_ptr<DX12::DepthStencilBuffer> DX12::Master::CreateDepthStencilBuffer(UINT64 width, UINT64 height, ResourceHeapType heap_type)
{
	return pimple_->CreateDepthStencilBuffer(width, height, heap_type);
}

void DX12::Master::CreateConstBufferView(std::shared_ptr<ConstBuffer> const_buff, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	pimple_->CreateConstBufferView(const_buff, desc_heap, index);
}

void DX12::Master::CreateRenderTargetView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	pimple_->CreateRenderTargetView(shader_resource, desc_heap, index);
}

void DX12::Master::CreateShaderResourceView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	pimple_->CreateShaderResourceView(shader_resource, desc_heap, index);
}

void DX12::Master::CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	pimple_->CreateDepthStencilBufferView(dsbuffer, desc_heap, index);
}

std::shared_ptr<DX12::RootSignature> DX12::Master::CreateRootSignature(int root_param_num)
{
	return pimple_->CreateRootSignature(root_param_num);
}

std::shared_ptr<DX12::GraphicsPipeline> DX12::Master::CreateGraphicsPipeline(std::shared_ptr<DX12::ShaderObject> vertex_shader, std::shared_ptr<DX12::ShaderObject> pixel_shader, DX12::VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer, DX12::PrimitiveTopology topology, std::shared_ptr<DX12::RootSignature> root_signature, LPCWSTR name)
{
	return pimple_->CreateGraphicsPipeline(vertex_shader, pixel_shader, layout_unit_array,
		layout_unit_num, dsbuffer, topology, root_signature, name);
}

std::shared_ptr<DX12::ShaderResource> DX12::Master::LoadTexture(LPCWSTR filename, LPCWSTR debug_name)
{
	return pimple_->LoadTexture(filename, debug_name);
}

void DX12::Master::SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
	ResourceBarrierState before, ResourceBarrierState after)
{
	pimple_->SetResourceBarrier(swapchain, before, after);
}

void DX12::Master::SetResourceBarrier(std::shared_ptr<ShaderResource> shader_resource,
	ResourceBarrierState before, ResourceBarrierState after)
{
	pimple_->SetResourceBarrier(shader_resource, before, after);
}

void DX12::Master::AddRootParameterAsDescriptorTable(std::shared_ptr<RootSignature> root_signature, std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis)
{
	pimple_->AddRootParameterAsDescriptorTable(root_signature, ranges, vis);
}

void* DX12::Master::Map(std::shared_ptr<VertexBuffer> vert_buffer)
{
	return pimple_->Map(vert_buffer);
}

unsigned int* DX12::Master::Map(std::shared_ptr<IndexBuffer> index_buffer)
{
	return pimple_->Map(index_buffer);
}

void* DX12::Master::Map(std::shared_ptr<ConstBuffer> const_buffer)
{
	return pimple_->Map(const_buffer);
}

void DX12::Master::Unmap(std::shared_ptr<VertexBuffer> vert_buffer)
{
	pimple_->Unmap(vert_buffer);
}

void DX12::Master::Unmap(std::shared_ptr<IndexBuffer> index_buffer)
{
	pimple_->Unmap(index_buffer);
}

void DX12::Master::Unmap(std::shared_ptr<ConstBuffer> const_buffer) {
	pimple_->Unmap(const_buffer);
}

void DX12::Master::Serialize(std::shared_ptr<RootSignature> root_signature)
{
	pimple_->Serialize(root_signature);
}

void DX12::Master::ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b)
{
	pimple_->ClearRenderTarget(swapchain, r, g, b);
}

void DX12::Master::ClearDepthStencilView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value)
{
	pimple_->ClearDepthStencilView(desc_heap, index, depth_value);
}

void DX12::Master::SetRenderTarget(std::shared_ptr<SwapChain> swapchain)
{
	pimple_->SetRenderTarget(swapchain);
}

void DX12::Master::SetRenderTarget(std::shared_ptr<SwapChain> swapchain, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	pimple_->SetRenderTarget(swapchain, desc_heap, index);
}

void DX12::Master::SetVertexBuffer(std::shared_ptr<VertexBuffer> vert_buffer, int slot_id)
{
	pimple_->SetVertexBuffer(vert_buffer, slot_id);
}

void DX12::Master::SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer)
{
	pimple_->SetIndexBuffer(index_buffer);
}

void DX12::Master::SetRootSignature(std::shared_ptr<RootSignature> root_signature)
{
	pimple_->SetRootSignature(root_signature);
}

void DX12::Master::SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap)
{
	pimple_->SetDescriptorHeap(desc_heap);
}

void DX12::Master::SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap, int base_desc_heap_index)
{
	pimple_->SetGraphicsRootDescriptorTable(root_param_index, desc_heap, base_desc_heap_index);
}

void DX12::Master::SetGraphicsPipeline(std::shared_ptr<GraphicsPipeline> pipeline)
{
	pimple_->SetGraphicsPipeline(pipeline);
}

void DX12::Master::SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth)
{
	pimple_->SetViewports(top_left_x, top_left_y, width, height, min_depth, max_depth);
}

void DX12::Master::SetScissorRect(LONG top_left_x, LONG top_left_y, LONG bottom_right_x, LONG bottom_right_y)
{
	pimple_->SetScissorRect(top_left_x, top_left_y, bottom_right_x, bottom_right_y);
}

void DX12::Master::DrawInstanced(int vertex_num, int start_vertex_loc)
{
	pimple_->DrawInstanced(vertex_num, start_vertex_loc);
}

void DX12::Master::DrawIndexedInstanced(int index_num, int start_index_loc)
{
	pimple_->DrawIndexedInstanced(index_num, start_index_loc);
}

void DX12::Master::Flip(std::shared_ptr<SwapChain> swapchain)
{
	pimple_->Flip(swapchain);
}

void DX12::Master::CleanUp()
{
	pimple_->CleanUp();
	delete pimple_;
}
