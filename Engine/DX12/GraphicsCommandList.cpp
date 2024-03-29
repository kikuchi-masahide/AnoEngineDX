//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GraphicsCommandList.h"

#include "DescriptorHeap.h"
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Buffer.h"
#include "RootSignature.h"
#include "ImageLoad.h"
#include "Texture2D.h"
#include "Texture1D.h"
#include "Log.h"

DX12::GraphicsCommandList::GraphicsCommandList()
{
}

DX12::GraphicsCommandList::GraphicsCommandList(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE cmdlist_type)
{
	auto res = device->CreateCommandAllocator(cmdlist_type, IID_PPV_ARGS(cmd_allocator_.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		Log::OutputCritical("Initialization of ID3D12CommandAllocator failed");
		throw 0;
	}
	res = device->CreateCommandList(0, cmdlist_type, cmd_allocator_.Get(), nullptr,
		IID_PPV_ARGS(cmd_list_.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		Log::OutputCritical("Initialization of ID3D12GraphicsCommandList failed");
		throw 0;
	}
}

void DX12::GraphicsCommandList::SetDebugName(LPCWSTR debug_name)
{
#ifdef _DEBUG
	cmd_list_->SetName(debug_name);
#endif
}

void DX12::GraphicsCommandList::ClearRenderTargetView(DescriptorHeap desc_heap, int index,
	float r, float g, float b)
{
	assert(desc_heap.GetHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	float color[4] = { r,g,b,1.0f };
	cmd_list_->ClearRenderTargetView(desc_heap.GetCPUDescriptorHandle(index), color, 0, nullptr);
}

void DX12::GraphicsCommandList::ClearDepthStencilBufferView(DescriptorHeap desc_heap, int index, float depth_value)
{
	assert(desc_heap.GetHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	cmd_list_->ClearDepthStencilView(desc_heap.GetCPUDescriptorHandle(index),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth_value, 0.0f, 0, nullptr);
}

void DX12::GraphicsCommandList::SetResourceBarrier(ResourceBarrierUnit unit)
{
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = unit.resource_.GetRawPtr();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = unit.before_;
	barrier.Transition.StateAfter = unit.after_;
	cmd_list_->ResourceBarrier(1, &barrier);
}

void DX12::GraphicsCommandList::SetResourceBarrier(const std::vector<ResourceBarrierUnit>& units)
{
	D3D12_RESOURCE_BARRIER* arr = DBG_NEW D3D12_RESOURCE_BARRIER[units.size()];
	for (int n = 0; n < units.size(); n++) {
		D3D12_RESOURCE_BARRIER& barrier = arr[n];
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = units[n].resource_.GetRawPtr();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = units[n].before_;
		barrier.Transition.StateAfter = units[n].after_;
	}
	cmd_list_->ResourceBarrier(units.size(), arr);
	delete arr;
}

void DX12::GraphicsCommandList::CopyResource(Resource dst, Resource src)
{
	cmd_list_->CopyResource(dst.GetRawPtr(), src.GetRawPtr());
}

void DX12::GraphicsCommandList::CopyBufferRegion(Buffer dst, UINT64 dst_offset,
	Buffer src, UINT64 src_offset, UINT64 copy_size)
{
	cmd_list_->CopyBufferRegion(dst.GetRawPtr(), dst_offset, src.GetRawPtr(), src_offset, copy_size);
}

void DX12::GraphicsCommandList::CopyBufferToTexture2D(Buffer buffer,
	UINT img_width, UINT img_height, DXGI_FORMAT img_format, UINT img_rowpitch, Texture2D texture)
{
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT srcfootprint;
	srcfootprint.Offset = 0;
	srcfootprint.Footprint.Format = img_format;
	srcfootprint.Footprint.Width = img_width;
	srcfootprint.Footprint.Height = img_height;
	srcfootprint.Footprint.Depth = 1;
	srcfootprint.Footprint.RowPitch = GetAlignmentedRowPitch(img_rowpitch);
	CD3DX12_TEXTURE_COPY_LOCATION src(buffer.GetRawPtr(), srcfootprint);
	CD3DX12_TEXTURE_COPY_LOCATION dst(texture.GetRawPtr(), 0);
	cmd_list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
}

void DX12::GraphicsCommandList::CopyBufferToTexture1D(Buffer buffer, UINT img_width,
	DXGI_FORMAT img_format, UINT img_rowpitch, Texture1D texture)
{
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT srcfootprint;
	srcfootprint.Offset = 0;
	srcfootprint.Footprint.Format = img_format;
	srcfootprint.Footprint.Width = img_width;
	srcfootprint.Footprint.Height = 1;
	srcfootprint.Footprint.Depth = 1;
	srcfootprint.Footprint.RowPitch = GetAlignmentedRowPitch(img_rowpitch);
	CD3DX12_TEXTURE_COPY_LOCATION src(buffer.GetRawPtr(), srcfootprint);
	CD3DX12_TEXTURE_COPY_LOCATION dst(texture.GetRawPtr(), 0);
	cmd_list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
}

void DX12::GraphicsCommandList::SetRenderTargets(DescriptorHeap desc_heap, int index,
	int num)
{
	assert(desc_heap.GetHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	auto handle = desc_heap.GetCPUDescriptorHandle(index);
	cmd_list_->OMSetRenderTargets(num, &handle, true, nullptr);
}

void DX12::GraphicsCommandList::SetRenderTargets(DescriptorHeap rtv_desc_heap,
	int rtv_index, int num, DescriptorHeap dsv_desc_heap, int dsv_index)
{
	assert(rtv_desc_heap.GetHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	assert(dsv_desc_heap.GetHeapType() == D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	auto rtv_handle = rtv_desc_heap.GetCPUDescriptorHandle(rtv_index);
	auto dsv_handle = dsv_desc_heap.GetCPUDescriptorHandle(dsv_index);
	cmd_list_->OMSetRenderTargets(num, &rtv_handle, true, &dsv_handle);
}

void DX12::GraphicsCommandList::SetVertexBuffer(VertexBuffer vert_buff, int slot_id)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	vbView.BufferLocation = vert_buff.GetGPUVirtualAddress();
	vbView.SizeInBytes = vert_buff.GetSizePerVert() * vert_buff.GetVertexNum();
	vbView.StrideInBytes = vert_buff.GetSizePerVert();
	cmd_list_->IASetVertexBuffers(slot_id, 1, &vbView);
}

void DX12::GraphicsCommandList::SetIndexBuffer(IndexBuffer index_buff)
{
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ibView.BufferLocation = index_buff.GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeof(unsigned int) * index_buff.GetVertexNum();
	cmd_list_->IASetIndexBuffer(&ibView);
}

void DX12::GraphicsCommandList::SetRootSignature(RootSignature root_signature)
{
	cmd_list_->SetGraphicsRootSignature(root_signature.GetRawPtr());
}

void DX12::GraphicsCommandList::SetDescriptorHeap(DescriptorHeap desc_heap)
{
	auto pdesc = desc_heap.GetRawPtr();
	cmd_list_->SetDescriptorHeaps(1, &pdesc);
}

void DX12::GraphicsCommandList::SetDescriptorHeaps(std::vector<DescriptorHeap> desc_heaps)
{
	ID3D12DescriptorHeap** arr = DBG_NEW ID3D12DescriptorHeap*[desc_heaps.size()];
	for (int n = 0; n < desc_heaps.size(); n++) {
		arr[n] = desc_heaps[n].GetRawPtr();
	}
	cmd_list_->SetDescriptorHeaps(desc_heaps.size(), arr);
	delete[] arr;
}

void DX12::GraphicsCommandList::SetGraphicsRootDescriptorTable(int root_param_index,
	DescriptorHeap desc_heap, int base_desc_heap_index)
{
	cmd_list_->SetGraphicsRootDescriptorTable(root_param_index,
		desc_heap.GetGPUDescriptorHandle(base_desc_heap_index));
}

void DX12::GraphicsCommandList::SetGraphicsRootCBV(int root_param_index, Buffer const_buffer)
{
	cmd_list_->SetGraphicsRootConstantBufferView(root_param_index, const_buffer.GetGPUVirtualAddress());
}

void DX12::GraphicsCommandList::SetGraphicsRootConstant(int root_param_index, SIZE_T size_to_set, void* src, int offset)
{
	int reg_num = static_cast<int>(size_to_set + 3) >> 2;
	cmd_list_->SetGraphicsRoot32BitConstants(root_param_index, reg_num, src, offset);
}

void DX12::GraphicsCommandList::SetGraphicsPipeline(GraphicsPipeline pipeline)
{
	cmd_list_->SetPipelineState(pipeline.GetRawPtr());
	cmd_list_->IASetPrimitiveTopology(pipeline.GetPrimitiveTopology());
}

void DX12::GraphicsCommandList::SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth)
{
	CD3DX12_VIEWPORT viewport(top_left_x, top_left_y, width, height, min_depth, max_depth);
	cmd_list_->RSSetViewports(1, &viewport);
}

void DX12::GraphicsCommandList::SetScissorRect(LONG top_left_x, LONG top_left_y, LONG bottom_right_x, LONG bottom_right_y)
{
	CD3DX12_RECT rect(top_left_x, top_left_y, bottom_right_x, bottom_right_y);
	cmd_list_->RSSetScissorRects(1, &rect);
}

void DX12::GraphicsCommandList::DrawInstanced(int vertex_num, int start_vertex_loc)
{
	cmd_list_->DrawInstanced(vertex_num, 1, start_vertex_loc, 0);
}

void DX12::GraphicsCommandList::DrawIndexedInstanced(unsigned int index_count_per_instance, unsigned int instance_count, unsigned int start_index_location, int base_vertex_location, unsigned int start_instance_location)
{
	cmd_list_->DrawIndexedInstanced(index_count_per_instance, instance_count, start_index_location,
		base_vertex_location, start_instance_location);
}

void DX12::GraphicsCommandList::Close()
{
	cmd_list_->Close();
}

void DX12::GraphicsCommandList::ResetCommandList()
{
	cmd_list_->Reset(cmd_allocator_.Get(), nullptr);
}

void DX12::GraphicsCommandList::ResetCommandAllocator()
{
	cmd_allocator_->Reset();
}

bool DX12::GraphicsCommandList::IsValid() const
{
	return cmd_list_;
}

ID3D12GraphicsCommandList* DX12::GraphicsCommandList::GetCommandListRawPtr() const
{
	return cmd_list_.Get();
}
