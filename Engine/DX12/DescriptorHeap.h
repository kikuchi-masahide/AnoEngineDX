//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"

namespace DX12 {
	class ConstBuffer;
	class ShaderResource;
	class DepthStencilBuffer;
	class DescriptorHeap {
	public:
		DescriptorHeap(ComPtr<ID3D12Device> device, int capacity, DescriptorHeapType type, DescriptorHeapShaderVisibility vis, LPCWSTR debug_name);
		void CreateConstBufferView(ComPtr<ID3D12Device> device,
			std::shared_ptr<ConstBuffer> const_buff, int index);
		void CreateRenderTargetView(ComPtr<ID3D12Device> device,
			std::shared_ptr<ShaderResource> shader_resource, int index);
		void CreateShaderResourceView(ComPtr<ID3D12Device> device,
			std::shared_ptr<ShaderResource> shader_resource, int index);
		void CreateDepthStencilBufferView(ComPtr<ID3D12Device> device,
			std::shared_ptr<DepthStencilBuffer> buffer, int index);
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int index);
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(int index);
		ID3D12DescriptorHeap* GetRawPtr();
		int const capacity_;
		DescriptorHeapType const heap_type_;
		DescriptorHeapShaderVisibility const shader_visibility_;
	private:
		ComPtr<ID3D12DescriptorHeap> desc_heap_;
		SIZE_T descriptor_handle_size_;
	};
}