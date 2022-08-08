//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"

/// <summary>
/// D3D12_HEAP_PROPERTIESとD3D12_RESOURCE_DESCからリソースを作成し、共通の操作を実装する
/// </summary>
namespace DX12 {
	class Resource
	{
	public:
		Resource(D3D12_HEAP_PROPERTIES heap_prop, D3D12_RESOURCE_DESC resource_desc);
		void Map();
		void Copy(void* src_begin,void* dst_begin, SIZE_T size);
		void Unmap();
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	private:
		ComPtr<ID3D12Resource> resource_;
	};
}
