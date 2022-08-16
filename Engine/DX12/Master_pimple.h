//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"
#include "SwapChain.h"
#include "ShaderObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "ConstBuffer.h"
#include "DescriptorHeap.h"
#include "ShaderResource.h"
#include "DepthStencilBuffer.h"

namespace DX12 {
	//HACK:3D/copyのCommandListを作り、画像の読み込み等でのcmdlist処理を分ける?
	//HACK:DX12関連のオブジェクトのメモリプールの使用について考える
	class Master_pimple final :public boost::noncopyable
	{
	public:
		void Initialize();
		void ProcessCommands();
		void ResetCmdAllocator();
		void ExecuteCmdLists();
		void FenceWaitingInProccessCommands();
		std::shared_ptr<ShaderObject> LoadShader(LPCWSTR filename, DX12::ShaderType shader_type);
		std::shared_ptr<SwapChain> CreateSwapChain(HWND hwnd, UINT width, UINT height);
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name);
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(int vert_num, LPCWSTR debug_name);
		std::shared_ptr<DescriptorHeap> CreateDescriptorHeap(int capacity, DescriptorHeapType type, DescriptorHeapShaderVisibility vis, LPCWSTR debug_name);
		std::shared_ptr<ConstBuffer> CreateConstBuffer(SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name);
		std::shared_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(UINT64 width, UINT64 height, ResourceHeapType heap_type);
		void CreateConstBufferView(std::shared_ptr<ConstBuffer> const_buff, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		void CreateRenderTargetView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		void CreateShaderResourceView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		void CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		std::shared_ptr<RootSignature> CreateRootSignature(int root_param_num);
		std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(
			std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
			VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer,
			PrimitiveTopology topology, std::shared_ptr<RootSignature> root_signature, LPCWSTR name);
		std::shared_ptr<ShaderResource> LoadTexture(LPCWSTR filename, LPCWSTR debug_name);
		void SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
			ResourceBarrierState before, ResourceBarrierState after);
		void SetResourceBarrier(std::shared_ptr<ShaderResource> shader_resource,
			ResourceBarrierState before, ResourceBarrierState after);
		void AddRootParameterAsDescriptorTable(std::shared_ptr<RootSignature> root_signature,
			std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis);
		void AddRootParameterAsCBV(std::shared_ptr<RootSignature> root_signature,
			UINT shader_register, RootParameterShaderVisibility vis);
		void AddRootParameterAsSRV(std::shared_ptr<RootSignature> root_signature,
			UINT shader_register, RootParameterShaderVisibility vis);
		void AddRootParameterAsConstant(std::shared_ptr<RootSignature> root_signature,
			UINT shader_register, SIZE_T const_size, RootParameterShaderVisibility vis);
		void Serialize(std::shared_ptr<RootSignature> root_signature);
		void* Map(std::shared_ptr<VertexBuffer> vert_buffer);
		unsigned int* Map(std::shared_ptr<IndexBuffer> index_buffer);
		void* Map(std::shared_ptr<ConstBuffer> const_buffer);
		void Unmap(std::shared_ptr<VertexBuffer> vert_buffer);
		void Unmap(std::shared_ptr<IndexBuffer> index_buffer);
		void Unmap(std::shared_ptr<ConstBuffer> const_buffer);
		void ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b);
		void ClearDepthStencilView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value);
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain);
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer, int slot_id);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer);
		void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
		void SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap);
		void SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap, int base_desc_heap_index);
		void SetGraphicsRootCBV(int root_param_index, std::shared_ptr<ConstBuffer> const_buffer);
		void SetGraphicsRootSRV(int root_param_index, std::shared_ptr<ShaderResource> shader_resource);
		void SetGraphicsRootConstant(int root_param_index, SIZE_T size_to_set, void* src, int offset);
		void SetGraphicsPipeline(std::shared_ptr<GraphicsPipeline> pipeline);
		void SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth);
		void SetScissorRect(LONG top_left_x, LONG top_left_y, LONG bottom_right_x, LONG bottom_right_y);
		void DrawInstanced(int vertex_num,int start_vertex_loc);
		void DrawIndexedInstanced(int index_num, int start_index_loc);
		void Flip(std::shared_ptr<SwapChain> swapchain);
		void CleanUp();
	private:
		void InitDebugLayer();
		void InitDevice();
		void InitFactory();
		void InitCmdAllocator_List_Queue();
		void InitDirectXTex();
		void InitDebugSetting();
		ComPtr<ID3D12CommandAllocator> cmd_allocator_;
		ComPtr<ID3D12GraphicsCommandList> cmd_list_;
		ComPtr<ID3D12CommandQueue> cmd_queue_;
		ComPtr<ID3D12Device> device_;
		ComPtr<ID3D12InfoQueue> info_queue_;
		ComPtr<IDXGIFactory6> factory_;
	};
}

