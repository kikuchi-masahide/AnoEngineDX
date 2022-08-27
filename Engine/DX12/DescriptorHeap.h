//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class ShaderResource;
	class ConstBuffer;
	class DepthStencilBuffer;
	class DescriptorHeap final:public boost::noncopyable {
	public:
		/// <param name="capacity">�ۗL����descriptor�̐�</param>
		/// <param name="type">
		/// CBV_SRV_UAV,RTV,DSV���w��(SAMPLER�͂܂�������)
		/// </param>
		/// <param name="vis">
		/// NONE��SHADER_VISIBLE���w��
		/// </param>
		DescriptorHeap(ComPtr<ID3D12Device> device, int capacity, D3D12_DESCRIPTOR_HEAP_TYPE type,
			D3D12_DESCRIPTOR_HEAP_FLAGS vis);
		//_DEBUG���̂ݗL��
		void SetDebugName(LPCWSTR debug_name);
		void CreateRenderTargetView(ComPtr<ID3D12Device> device,
			std::shared_ptr<ShaderResource> shader_resource, int index);
		void CreateConstBufferView(ComPtr<ID3D12Device> device,
			std::shared_ptr<ConstBuffer> buffer, int index);
		void CreateShaderResourceView(ComPtr<ID3D12Device> device,
			std::shared_ptr<ShaderResource> shader_resource, int index);
		void CreateDepthStencilBufferView(ComPtr<ID3D12Device> device,
			std::shared_ptr<DepthStencilBuffer> buffer, int index);
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int index) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(int index) const;
		ID3D12DescriptorHeap* GetRawPtr() const;
		int const capacity_;
		D3D12_DESCRIPTOR_HEAP_TYPE const heap_type_;
		D3D12_DESCRIPTOR_HEAP_FLAGS const shader_visibility_;
	private:
		ComPtr<ID3D12DescriptorHeap> desc_heap_;
		SIZE_T descriptor_handle_size_;
	};
}