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
		/// index bufferを作成する
		/// </summary>
		/// <param name="state">通常GENERIC_READ</param>
		/// <param name="vert_num">含まれる全頂点の数 例えば三角形の場合、全頂点は3×三角形の数</param>
		IndexBuffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state,
			int vert_num);
		int const vertex_num_;
	};
}