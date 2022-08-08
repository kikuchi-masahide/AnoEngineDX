//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"

namespace DX12 {
	/// <summary>
	/// Vertex型のクラス/構造体を1頂点分のデータとする頂点バッファ
	/// </summary>
	class VertexBuffer final:public boost::noncopyable
	{
	public:
		VertexBuffer(ComPtr<ID3D12Device> device, SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name);
		void* Map();
		void Unmap();
		void SetVertexBuffer(ComPtr<ID3D12GraphicsCommandList> list, int slotid);
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const;
		SIZE_T const size_per_vert_;
		int const vertex_num_;
	private:
		ComPtr<ID3D12Resource> resource_;
	};
}