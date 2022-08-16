//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "ConfigEnums.h"
#include "VertexLayout.h"
#include "DescriptorRange.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace DX12 {
	class Master_pimple;
	class SwapChain;
	class ShaderObject;
	class VertexBuffer;
	class IndexBuffer;
	class RootSignature;
	class GraphicsPipeline;
	class ConstBuffer;
	class DescriptorHeap;
	class ShaderResource;
	class DepthStencilBuffer;
	class Master final:public boost::noncopyable
	{
	public:
		void Initialize();
		void ProcessCommands();
		/// <summary>
		/// ���̃V�F�[�_��ǂݍ���ShaderObject���쐬����
		/// </summary>
		std::shared_ptr<ShaderObject> LoadShader(LPCWSTR filename, DX12::ShaderType shader_type);
		std::shared_ptr<SwapChain> CreateSwapChain(HWND hwnd, UINT width, UINT height);
		/// <summary>
		/// ���_�o�b�t�@�����
		/// </summary>
		/// <param name="size_per_vert">1���_��\���\���̂̃T�C�Y</param>
		/// <param name="vert_num">�S���_��</param>
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name);
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�����
		/// </summary>
		/// <param name="vert_num">�z��Ɋ܂܂�鑍���_��(�O�p�`�̃|���S����ɕt���O���_)</param>
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(int vert_num, LPCWSTR debug_name);
		/// <summary>
		/// �萔�o�b�t�@�����
		/// </summary>
		/// <param name="size">�o�b�t�@�S�̂̃T�C�Y(�����I��256�A���C�������g����)</param>
		/// <param name="heap_type">�A�N�Z�X�^�C�v</param>
		std::shared_ptr<ConstBuffer> CreateConstBuffer(SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name);
		/// <summary>
		/// Depth Stencil Buffer�����
		/// </summary>
		/// <param name="width">px�P�ʂł̕�(�����_�[�^�[�Q�b�g�ɍ��킹��)</param>
		/// <param name="height">px�P�ʂł̍���(�����_�[�^�[�Q�b�g�ɍ��킹��)</param>
		/// <param name="heap_type">Heap Type(�ʏ�GPU���炵�����삵�Ȃ�����DEFAULT)</param>
		std::shared_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(UINT64 width, UINT64 height, ResourceHeapType heap_type = ResourceHeapType::DEFAULT);
		/// <summary>
		/// DescriptorHeap�����
		/// </summary>
		/// <param name="capacity">���̃q�[�v������Descriptor�̐�</param>
		/// <param name="type">�i�[����Descriptor�̎��</param>
		/// <param name="vis">Shader����̉���</param>
		std::shared_ptr<DescriptorHeap> CreateDescriptorHeap(int capacity, DescriptorHeapType type, DescriptorHeapShaderVisibility vis, LPCWSTR debug_name);
		/// <summary>
		/// ����DescriptorHeap�ɁAconst_buff��CBV���쐬����
		/// </summary>
		/// <param name="index">desc_heap�̉��Ԗ�(0-indexed)�ɍ쐬���邩</param>
		void CreateConstBufferView(std::shared_ptr<ConstBuffer> const_buff, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// ����DescriptorHeap�ɁAshader_resource��RTV���쐬����
		/// </summary>
		/// <param name="index">desc_heap�̉��Ԗ�(0-indexed)�ɍ쐬���邩</param>
		void CreateRenderTargetView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// ����DescriptorHeap�ɁAshader_resource��SRV���쐬����
		/// </summary>
		/// <param name="index">desc_heap�̉��Ԗ�(0-indexed)�ɍ쐬���邩</param>
		void CreateShaderResourceView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// ����DesciptorHeap�ɁAdsbuffer��DSV���쐬����
		/// </summary>
		/// <param name="index">desc_heap�̉��Ԗ�(0-indexed)�ɍ쐬���邩</param>
		void CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		//1�̃��[�g�V�O�l�`����\���I�u�W�F�N�g���쐬����
		std::shared_ptr<RootSignature> CreateRootSignature(int root_param_num);
		/// <summary>
		/// �O���t�B�b�N�p�C�v���C����\���I�u�W�F�N�g���쐬����
		/// </summary>
		/// <param name="dsbuffer">Depth Stencil Buffer��L���ɂ��邩�ۂ�</param>
		/// <param name="topology">����Graphics Pipeline�ŕ`�悷��`��</param>
		std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(
			std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
			VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer,
			PrimitiveTopology topology, std::shared_ptr<RootSignature> root_signature, LPCWSTR name);
		/// <summary>
		/// ���̃t�@�C����ǂݍ��݁A�t�@�C���f�[�^��ۗL����ShaderResource��Ԃ�
		/// </summary>
		/// <param name="debug_name">�R�s�[�O/��̃o�b�t�@�̃f�o�b�O�p���O</param>
		std::shared_ptr<ShaderResource> LoadTexture(LPCWSTR filename, LPCWSTR debug_name);
		/// <summary>
		/// swapchain�̍��̃o�b�N�o�b�t�@�Ƀo���A�𒣂�
		/// </summary>
		void SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
			ResourceBarrierState before, ResourceBarrierState after);
		/// <summary>
		/// ShaderResource�Ƀo���A�𒣂�
		/// </summary>
		void SetResourceBarrier(std::shared_ptr<ShaderResource> shader_resource,
			ResourceBarrierState before, ResourceBarrierState after);
		/// <summary>
		/// ����RootSignature�ɁADescriptorTable�^�C�v��RootParameter��ǉ�����
		/// </summary>
		void AddRootParameterAsDescriptorTable(std::shared_ptr<RootSignature> root_signature,
			std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis);
		/// <summary>
		/// ���̒��_�o�b�t�@�ɏ����R�s�[���邽�߂̃|�C���^���擾����
		/// </summary>
		void* Map(std::shared_ptr<VertexBuffer> vert_buffer);
		/// <summary>
		/// ���̃C���f�b�N�X�o�b�t�@�ɏ����R�s�[���邽�߂̃|�C���^���擾����
		/// </summary>
		unsigned int* Map(std::shared_ptr<IndexBuffer> index_buffer);
		void* Map(std::shared_ptr<ConstBuffer> const_buffer);
		void Unmap(std::shared_ptr<VertexBuffer> vert_buffer);
		void Unmap(std::shared_ptr<IndexBuffer> index_buffer);
		void Unmap(std::shared_ptr<ConstBuffer> const_buffer);
		/// <summary>
		/// ���[�g�V�O�l�`�����V���A���C�Y���� �ȍ~���[�g�p�����^��T���v���[�̕ύX�͂ł��Ȃ�
		/// </summary>
		void Serialize(std::shared_ptr<RootSignature> root_signature);
		/// <summary>
		/// �R�}���h���X�g�ɍ��̃o�b�N�o�b�t�@�̃N���A���߂𓊂���
		/// </summary>
		void ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b);
		/// <summary>
		/// desc_heap��index�Ԗڂ�DSV�̎w��Depth Stencil Buffer���Adepth_value�ŃN���A����
		/// </summary>
		void ClearDepthStencilView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value = 1.0f);
		/// <summary>
		/// swapchain�̍��̃o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�Ɏw�肷��(Depth Stencil Buffer���Z�b�g���Ȃ�)
		/// </summary>
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain);
		/// <summary>
		/// swapchain�̍��̃o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�Ɏw�肷��
		/// (desc_heap��index�Ԗڂɑ��݂���DSV���w��Depth Stencil Buffer���Z�b�g����)
		/// </summary>
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vert_buffer, int slot_id = 0);
		/// <summary>
		/// ����IndexBuffer���C���f�b�N�X�o�b�t�@�Ƃ��ăZ�b�g����
		/// </summary>
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer);
		/// <summary>
		/// ����RootSignature���Z�b�g����
		/// </summary>
		void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
		/// <summary>
		/// ����DescriptorHeap���Z�b�g����
		/// (SetGraphicsRootDescriptorTable�Ƃ͕ʂɁA�p����DescriptorHeap���Z�b�g����K�v������)
		/// </summary>
		/// <param name="desc_heap"></param>
		void SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap);
		/// <summary>
		/// ����DescriptorHeap��RootParameter�ɑΉ�������
		/// </summary>
		/// <param name="root_param_index">���Ԗڂ�RootParameter��</param>
		/// <param name="base_desc_heap_index">desc_heap�̉��Ԗڂ�Descriptor������Z�b�g���邩</param>
		void SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap, int base_desc_heap_index = 0);
		/// <summary>
		/// ����GraphicsPipeline�Ƃ���̏]��PrimitiveTopology���Z�b�g����
		/// </summary>
		void SetGraphicsPipeline(std::shared_ptr<GraphicsPipeline> pipeline);
		/// <summary>
		/// �����_�[�^�[�Q�b�g�̂����A�ǂ̒����`�����Ɏ������`�悷�邩�w�肷��
		/// </summary>
		/// <param name="top_left_x">�����_�[�^�[�Q�b�g�����(0,0)�Ƃ����Ƃ��́Apx�P�ʂł̍�x���W</param>
		/// <param name="top_left_y">�����_�[�^�[�Q�b�g�����(0,0)�Ƃ����Ƃ��́Apx�P�ʂł̏�y���W</param>
		/// <param name="width">px�P�ʂł̕�</param>
		/// <param name="height">px�P�ʂł̍���</param>
		/// <param name="min_depth">�ߐڕ��ʂ�z���W</param>
		/// <param name="max_depth">�����ʂ�z���W</param>
		void SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth = 0.0f, float max_depth = 1.0f);
		/// <summary>
		/// �����_�[�^�[�Q�b�g�̂����A�ǂ̒����`�������V�U�[���N�g�ɂ��邩�w�肷��
		/// �V�U�[���N�g�O�̃s�N�Z���͎̂Ă���
		/// </summary>
		/// <param name="top_left_x">�����(0,0)�Ƃ����Ƃ��́Apx�P�ʂł̍�x���W</param>
		/// <param name="top_left_y">�����(0,0)�Ƃ����Ƃ��́Apx�P�ʂł̏�y���W</param>
		/// <param name="bottom_right_x">�����(0,0)�Ƃ����Ƃ��́Apx�P�ʂł̉Ex���W</param>
		/// <param name="bottom_right_y">�����(0,0)�Ƃ����Ƃ��́Apx�P�ʂł̉�y���W</param>
		void SetScissorRect(LONG top_left_x, LONG top_left_y, LONG bottom_right_x, LONG bottom_right_y);
		/// <param name="vertex_num">�`�悷�钸�_��</param>
		/// <param name="start_vertex_loc">�ǂ̒��_����`�悷�邩</param>
		void DrawInstanced(int vertex_num, int start_vertex_loc);
		/// <param name="index_num">�`�悷��C���f�b�N�X�̐�</param>
		/// <param name="start_index_loc">�ǂ̃C���f�b�N�X����`�悷�邩</param>
		void DrawIndexedInstanced(int index_num, int start_index_loc);
		/// <summary>
		/// ����swapchain��flip����
		/// </summary>
		void Flip(std::shared_ptr<SwapChain> swapchain);
		void CleanUp();
	private:
		Master_pimple* pimple_;
	};
}

