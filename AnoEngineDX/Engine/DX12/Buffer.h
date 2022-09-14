//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Resource.h"

namespace DX12 {
	class Buffer:public Resource{
	public:
		Buffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, SIZE_T size,
			D3D12_RESOURCE_STATES state);
		SIZE_T const size_;
	};
}