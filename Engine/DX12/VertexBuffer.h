//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Buffer.h"

namespace DX12 {
	class VertexBuffer final :public Buffer{
	public:
		VertexBuffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state,
			SIZE_T size_per_vert, int vert_num);
		SIZE_T const size_per_vert_;
		int const vertex_num_;
	};
}