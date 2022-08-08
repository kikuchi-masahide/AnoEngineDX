//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"

namespace DX12 {
	class ConstBuffer final :boost::noncopyable {
	public:
		ConstBuffer(ComPtr<ID3D12Device> device, SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name);
		void* Map();
		void Unmap();
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
		//256アラインメント済みの、定数バッファのサイズ
		SIZE_T const whole_256ed_size_;
	private:
		ComPtr<ID3D12Resource> resource_;
	};
}