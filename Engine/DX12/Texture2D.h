//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Resource.h"

namespace DX12 {
	class Texture2D final :public Resource {
	public:
		//swapchain‚Ìindex”Ô–Ú‚Ìbackbuffer‚ðŽæ“¾ SwapChain‚Ì‚Ý‚ÅŽg—p
		Texture2D(ComPtr<IDXGISwapChain> swapchain, int index, DXGI_FORMAT dxgi_format);
		Texture2D(ComPtr<ID3D12Device> device, UINT64 width, UINT height,
			DXGI_FORMAT dxgi_format, D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout,
			D3D12_RESOURCE_STATES state);
		DXGI_FORMAT const dxgi_format_;
		UINT const width_;
		UINT const height_;
	};
}