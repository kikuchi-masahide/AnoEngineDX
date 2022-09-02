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
	/// Device��Factory���Ǘ����A�I�u�W�F�N�g�쐬�̒��S�ƂȂ�
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
		/// ���_�o�b�t�@�̍쐬
		/// </summary>
		/// <param name="state">�ʏ�GENERIC_READ</param>
		/// <param name="size_per_vert">�꒸�_������̃f�[�^�T�C�Y</param>
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(D3D12_HEAP_TYPE heap_type,
			D3D12_RESOURCE_STATES state, SIZE_T size_per_vert, int vert_num);
		/// <summary>
		/// index buffer���쐬����
		/// </summary>
		/// <param name="state">�ʏ�GENERIC_READ</param>
		/// <param name="vert_num">�܂܂��S���_�̐� �Ⴆ�ΎO�p�`�̏ꍇ�A�S���_��3�~�O�p�`�̐�</param>
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(D3D12_HEAP_TYPE heap_type,
			D3D12_RESOURCE_STATES state, int vert_num);
		/// <summary>
		/// �萔�o�b�t�@���쐬����(������256�A���C�������g�����)
		/// </summary>
		std::shared_ptr<ConstBuffer> CreateConstBuffer(D3D12_HEAP_TYPE heap_type, SIZE_T size,
			D3D12_RESOURCE_STATES state);
		/// <summary>
		/// �ꎟ�R�s�[�p�o�b�t�@�ȂǁA�ėp�I�ȃo�b�t�@���쐬����
		/// </summary>
		std::shared_ptr<Buffer> CreateBuffer(D3D12_HEAP_TYPE heap_type, SIZE_T size,
			D3D12_RESOURCE_STATES state);
		/// <summary>
		/// ���Texture2D�����
		/// </summary>
		/// <param name="texture_layout">�ꎞ�o�b�t�@����R�s�[����ꍇ��UNKNOWN</param>
		std::shared_ptr<Texture2D> CreateTexture2D(UINT64 width, UINT height,
			DXGI_FORMAT dxgi_format, D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout,
			D3D12_RESOURCE_STATES state);
		std::shared_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(UINT64 width, UINT height,
			D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT);
		std::shared_ptr<DescriptorHeap> CreateDescriptorHeap(int capacity, D3D12_DESCRIPTOR_HEAP_TYPE type,
			D3D12_DESCRIPTOR_HEAP_FLAGS vis);
		/// <summary>
		/// desc_heap��index�ԖڂɁAbuffer�ɑ΂���CBV���쐬
		/// </summary>
		void CreateConstBufferView(std::shared_ptr<ConstBuffer> buffer,
			std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// desc_heap��index�ԖڂɁAshader_resource�ɑ΂���SRV���쐬
		/// </summary>
		void CreateTexture2DView(std::shared_ptr<Texture2D> shader_resource,
			std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// desc_heap��index�ԖڂɁAdsbuffer�ɑ΂���DSV���쐬
		/// </summary>
		void CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer,
			std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// desc_heap��index�ԖڂɁAsampler���쐬
		/// </summary>
		/// <param name="address_u">u��0�`1�͈̔͊O�̂Ƃ��̋��� �ʏ�J��Ԃ�</param>
		/// <param name="address_v">v��0�`1�͈̔͊O�̂Ƃ��̋��� �ʏ�J��Ԃ�</param>
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