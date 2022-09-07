//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "VertexLayout.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace DX12 {
	class CommandQueue;
	class Fence;
	class GraphicsCommandList;
	class SwapChain;
	class DescriptorHeap;
	class VertexBuffer;
	class IndexBuffer;
	class ConstBuffer;
	class Texture2D;
	class DepthStencilBuffer;
	class Buffer;
	class ShaderObject;
	class RootSignature;
	class GraphicsPipeline;
	/// <summary>
	/// DeviceとFactoryを管理し、オブジェクト作成の中心となる
	/// </summary>
	class Master final :public boost::noncopyable {
	public:
		void Initialize();
		std::shared_ptr<CommandQueue> CreateCommandQueue(D3D12_COMMAND_LIST_TYPE cmdlist_type);
		std::shared_ptr<Fence> CreateFence(UINT64 value0 = 0);
		std::shared_ptr<GraphicsCommandList> CreateGraphicsCommandList(D3D12_COMMAND_LIST_TYPE cmdlist_type);
		std::shared_ptr<SwapChain> CreateSwapChain(std::shared_ptr<CommandQueue> cmd_queue, HWND hwnd,
			UINT width, UINT height);
		/// <summary>
		/// 頂点バッファの作成
		/// </summary>
		/// <param name="state">通常GENERIC_READ</param>
		/// <param name="size_per_vert">一頂点当たりのデータサイズ</param>
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(D3D12_HEAP_TYPE heap_type,
			D3D12_RESOURCE_STATES state, SIZE_T size_per_vert, int vert_num);
		/// <summary>
		/// index bufferを作成する
		/// </summary>
		/// <param name="state">通常GENERIC_READ</param>
		/// <param name="vert_num">含まれる全頂点の数 例えば三角形の場合、全頂点は3×三角形の数</param>
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(D3D12_HEAP_TYPE heap_type,
			D3D12_RESOURCE_STATES state, int vert_num);
		/// <summary>
		/// 定数バッファを作成する(内部で256アラインメントされる)
		/// </summary>
		std::shared_ptr<ConstBuffer> CreateConstBuffer(D3D12_HEAP_TYPE heap_type, SIZE_T size,
			D3D12_RESOURCE_STATES state);
		/// <summary>
		/// 一次コピー用バッファなど、汎用的なバッファを作成する
		/// </summary>
		std::shared_ptr<Buffer> CreateBuffer(D3D12_HEAP_TYPE heap_type, SIZE_T size,
			D3D12_RESOURCE_STATES state);
		/// <summary>
		/// 空のTexture2Dを作る
		/// </summary>
		/// <param name="texture_layout">一時バッファからコピーする場合はUNKNOWN</param>
		std::shared_ptr<Texture2D> CreateTexture2D(UINT64 width, UINT height,
			DXGI_FORMAT dxgi_format, D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout,
			D3D12_RESOURCE_STATES state);
		std::shared_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(UINT64 width, UINT height,
			D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT);
		std::shared_ptr<DescriptorHeap> CreateDescriptorHeap(int capacity, D3D12_DESCRIPTOR_HEAP_TYPE type,
			D3D12_DESCRIPTOR_HEAP_FLAGS vis);
		/// <summary>
		/// desc_heapのindex番目に、bufferに対するCBVを作成
		/// </summary>
		void CreateConstBufferView(std::shared_ptr<ConstBuffer> buffer,
			std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// desc_heapのindex番目に、shader_resourceに対するSRVを作成
		/// </summary>
		void CreateTexture2DView(std::shared_ptr<Texture2D> shader_resource,
			std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// desc_heapのindex番目に、dsbufferに対するDSVを作成
		/// </summary>
		void CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer,
			std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// desc_heapのindex番目に、samplerを作成
		/// </summary>
		/// <param name="address_u">uが0～1の範囲外のときの挙動 通常繰り返し</param>
		/// <param name="address_v">vが0～1の範囲外のときの挙動 通常繰り返し</param>
		void CreateSampler(std::shared_ptr<DescriptorHeap> desc_heap, int index,
			D3D12_TEXTURE_ADDRESS_MODE address_u = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE address_v = D3D12_TEXTURE_ADDRESS_MODE_WRAP);
		void Serialize(std::shared_ptr<RootSignature> root_signature);
		std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(
			std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
			const std::vector<VertexLayoutUnit>& vertex_layout, bool dsbuffer,
			D3D_PRIMITIVE_TOPOLOGY primitive_topology, std::shared_ptr<RootSignature> root_signature);
	private:
		void InitDebugLayer();
		void InitDevice();
		void InitFactory();
		void InitDirectXTex();
		void InitDebugSetting();
		ComPtr<ID3D12Device> device_;
		ComPtr<ID3D12InfoQueue> info_queue_;
		ComPtr<IDXGIFactory6> factory_;
	};
}