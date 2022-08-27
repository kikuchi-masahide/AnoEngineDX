//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Buffer.h"

namespace DX12 {
	class IndexBuffer:public Buffer {
	public:
		/// <summary>
		/// index buffer���쐬����
		/// </summary>
		/// <param name="state">�ʏ�GENERIC_READ</param>
		/// <param name="vert_num">�܂܂��S���_�̐� �Ⴆ�ΎO�p�`�̏ꍇ�A�S���_��3�~�O�p�`�̐�</param>
		IndexBuffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state,
			int vert_num);
		int const vertex_num_;
	};
}