//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"

namespace DX12 {
	class DepthStencilBuffer final :public boost::noncopyable {
	public:
		DepthStencilBuffer(ComPtr<ID3D12Device> device, UINT64 width, UINT64 height, ResourceHeapType heap_type = ResourceHeapType::DEFAULT);
		ID3D12Resource* GetRawPtr() const;
	private:
		ComPtr<ID3D12Resource> resource_;
	};
}