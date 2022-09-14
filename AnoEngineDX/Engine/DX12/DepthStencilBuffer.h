//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Resource.h"

namespace DX12 {
	class DepthStencilBuffer :public Resource {
	public:
		DepthStencilBuffer(ComPtr<ID3D12Device> device, UINT64 width, UINT height, D3D12_HEAP_TYPE heap_type);
	};
}