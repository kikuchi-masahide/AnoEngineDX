//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class Texture2D;
	class Texture1D;
	class Buffer;
	class DepthStencilBuffer;
	class DescriptorHeap final {
	public:
		DescriptorHeap();
		/// <param name="capacity">保有するdescriptorの数</param>
		/// <param name="type">
		/// CBV_SRV_UAV,RTV,DSVを指定(SAMPLERはまだ未調整)
		/// </param>
		/// <param name="vis">
		/// NONEかSHADER_VISIBLEを指定
		/// </param>
		DescriptorHeap(ComPtr<ID3D12Device> device, int capacity, D3D12_DESCRIPTOR_HEAP_TYPE type,
			D3D12_DESCRIPTOR_HEAP_FLAGS vis);
		//_DEBUG時のみ有効
		void SetDebugName(LPCWSTR debug_name);
		void CreateRenderTargetView(ComPtr<ID3D12Device> device,
			Texture2D shader_resource, int index);
		void CreateConstBufferView(ComPtr<ID3D12Device> device, Buffer buffer, int index);
		void CreateTexture2DView(ComPtr<ID3D12Device> device, Texture2D shader_resource, int index);
		void CreateTexture1DView(ComPtr<ID3D12Device> device,Texture1D shader_resource, int index);
		void CreateBufferView(ComPtr<ID3D12Device> device,
			Buffer shader_resource, DXGI_FORMAT dxgi_format, int num_element, int index);
		void CreateBufferView(ComPtr<ID3D12Device> device, Buffer shader_resource,
			size_t structure_byte_stride, int num_element, int index);
		void CreateDepthStencilBufferView(ComPtr<ID3D12Device> device,
			DepthStencilBuffer buffer, int index);
		void CreateSampler(ComPtr<ID3D12Device> device, int index,
			D3D12_TEXTURE_ADDRESS_MODE address_u, D3D12_TEXTURE_ADDRESS_MODE address_v);
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int index) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(int index) const;
		ID3D12DescriptorHeap* GetRawPtr() const;
		bool IsValid() const;
		int GetCapacity() const;
		D3D12_DESCRIPTOR_HEAP_TYPE GetHeapType() const;
		D3D12_DESCRIPTOR_HEAP_FLAGS GetShaderVisibility() const;
	private:
		ComPtr<ID3D12DescriptorHeap> desc_heap_;
		SIZE_T descriptor_handle_size_;
		int capacity_;
		D3D12_DESCRIPTOR_HEAP_TYPE heap_type_;
		D3D12_DESCRIPTOR_HEAP_FLAGS shader_visibility_;
	};
}