//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class DescriptorHeap;
	class Resource;
	class VertexBuffer;
	class IndexBuffer;
	class RootSignature;
	class GraphicsPipeline;
	class Buffer;
	class ShaderResource;
	/// <summary>
	/// resource barrier�J�ڂ̎w��Ɏg��
	/// </summary>
	struct ResourceBarrierUnit final{
	public:
		ResourceBarrierUnit(std::shared_ptr<Resource> r,D3D12_RESOURCE_STATES b,D3D12_RESOURCE_STATES a)
			:resource_(r),before_(b),after_(a)
		{}
		std::shared_ptr<Resource> resource_;
		D3D12_RESOURCE_STATES before_;
		D3D12_RESOURCE_STATES after_;
	};
	/// <summary>
	/// GraphicsCommandList��CommandAllocator���Ǘ�����
	/// CommandList��Allocator��1:1�őΉ�������
	/// </summary>
	class GraphicsCommandList final :public boost::noncopyable {
	public:
		//Master����Ăяo��
		GraphicsCommandList(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE cmdlist_type);
		void SetDebugName(LPCWSTR debug_name);
		/// <summary>
		/// desc_heap_��index�Ԗڂ�RTV���w��F�ŃN���A
		/// </summary>
		void ClearRenderTargetView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float r, float g, float b);
		/// <summary>
		/// desc_heap_��index�Ԗڂ�DSV���w��l�ŃN���A
		/// </summary>
		void ClearDepthStencilBufferView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value);
		void SetResourceBarrier(ResourceBarrierUnit unit);
		void SetResourceBarrier(const std::vector<ResourceBarrierUnit>& units);
		/// <summary>
		/// Resource�S�̂��R�s�[����
		/// </summary>
		void CopyResource(std::shared_ptr<Resource> dst, std::shared_ptr<Resource> src);
		/// <summary>
		/// Buffer�̈ꕔ����Buffer�փR�s�[����
		/// </summary>
		void CopyBufferRegion(std::shared_ptr<Resource> dst, UINT64 dst_offset,
			std::shared_ptr<Resource> src, UINT64 src_offset, UINT64 copy_size);
		void CopyBufferToTexture(std::shared_ptr<Buffer> buffer,
			UINT img_width,UINT img_height, DXGI_FORMAT img_format, UINT img_rowpitch,
			std::shared_ptr<ShaderResource> texture);
		/// <summary>
		/// num��RTV���Z�b�g����BDSV�͂Ȃ��B
		/// RTV��desc_heap��index�Ԗڂ���A�����ĕ���ł���Ƃ���B
		/// </summary>
		void SetRenderTargets(std::shared_ptr<DescriptorHeap> desc_heap, int index, int num);
		/// <summary>
		/// num��RTV���Z�b�g����BDSV��dsv_desc_heap��dsv_index�ԖځB
		/// RTV��rtv_desc_heap��rtv_index�Ԗڂ���A�����ĕ���ł���Ƃ���B
		/// </summary>
		void SetRenderTargets(std::shared_ptr<DescriptorHeap> rtv_desc_heap, int rtv_index, int num,
			std::shared_ptr<DescriptorHeap> dsv_desc_heap, int dsv_index);
		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vert_buff, int slot_id = 0);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buff);
		void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
		/// <summary>
		/// ����DescriptorHeap���Z�b�g����
		/// (SetGraphicsRootDescriptorTable�Ƃ͕ʂɁA�p����DescriptorHeap���Z�b�g����K�v������)
		/// </summary>
		void SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap);
		/// <summary>
		/// ����DescriptorHeap(������)���Z�b�g����
		/// (SetGraphicsRootDescriptorTable�Ƃ͕ʂɁA�p����DescriptorHeap���Z�b�g����K�v������)
		/// </summary>
		void SetDescriptorHeaps(std::vector<std::shared_ptr<DescriptorHeap>> desc_heaps);
		/// <summary>
		/// ����DescriptorHeap��RootParameter�ɑΉ�������
		/// </summary>
		/// <param name="root_param_index">���Ԗڂ�RootParameter��</param>
		/// <param name="base_desc_heap_index">desc_heap�̉��Ԗڂ�Descriptor������Z�b�g���邩</param>
		void SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap,
			int base_desc_heap_index = 0);
		/// <summary>
		/// ����ConstBuffer��RootParameter�ɑΉ�������
		/// </summary>
		/// <param name="root_param_index">RootSignature�̉��Ԗڂ�RootParameter��</param>
		void SetGraphicsRootCBV(int root_param_index, std::shared_ptr<Buffer> const_buffer);
		/// <summary>
		/// src����n�܂�f�[�^��RootSignature��root_param_index�Ԗڂ�RootSignature�ɃZ�b�g����
		/// </summary>
		void SetGraphicsRootConstant(int root_param_index, SIZE_T size_to_set, void* src, int offset = 0);
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
		void SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth);
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
		void DrawInstanced(int vertex_num, int start_vertex_loc = 0);
		/// <param name="index_num">�`�悷��C���f�b�N�X�̐�</param>
		/// <param name="start_index_loc">�ǂ̃C���f�b�N�X����`�悷�邩</param>
		void DrawIndexedInstanced(int index_num, int start_index_loc = 0);
		/// <summary>
		/// CommandList���N���[�Y����(record���~�߂�) execute�̑O�Ɏ��s����
		/// </summary>
		void Close();
		void ResetCommandList();
		void ResetCommandAllocator();
		ID3D12GraphicsCommandList* GetCommandListRawPtr() const;
	private:
		ComPtr<ID3D12GraphicsCommandList> cmd_list_;
		ComPtr<ID3D12CommandAllocator> cmd_allocator_;
	};
}