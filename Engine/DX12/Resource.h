//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	/// <summary>
	/// resource��\���ėp�N���X resource�ɋ��ʂ̑������������
	/// </summary>
	class Resource :public boost::noncopyable {
	public:
		//ShaderResource��SwapChain�ɑΉ����邽�߂̋�R���X�g���N�^
		Resource();
		/// <param name="heap_type">
		/// DEFAULT:CPU����A�N�Z�X�ł��Ȃ�
		/// UPLOAD:CPU����A�N�Z�X�ł���
		/// READBACK:CPU����ǂݎ���
		/// </param>
		Resource(ComPtr<ID3D12Device> device, D3D12_HEAP_FLAGS heap_flag, D3D12_HEAP_TYPE heap_type,
			D3D12_RESOURCE_DESC resource_desc, D3D12_RESOURCE_STATES state);
		//_DEBUG���̂ݗL��
		void SetDebugName(LPCWSTR debug_name);
		void* Map();
		void Unmap();
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const;
		ID3D12Resource* GetRawPtr() const;
	protected:
		ComPtr<ID3D12Resource> resource_;
	};
}