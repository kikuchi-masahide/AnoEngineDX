//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"

namespace DX12 {
	class IndexBuffer final :public boost::noncopyable
	{
	public:
		IndexBuffer(ComPtr<ID3D12Device> device, int vertex_num, LPCWSTR debug_name);
		unsigned int* Map();
		void Unmap();
		void SetIndexBuffer(ComPtr<ID3D12GraphicsCommandList> list);
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const;
		int vertex_num_;
	private:
		ComPtr<ID3D12Resource> resource_;
	};
}
