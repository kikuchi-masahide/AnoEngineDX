//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Resource.h"

namespace DX12 {
	class Texture2D final :public Resource {
	public:
		Texture2D();
		//swapchainのindex番目のbackbufferを取得 SwapChainのみで使用
		Texture2D(ComPtr<IDXGISwapChain> swapchain, int index, DXGI_FORMAT dxgi_format);
		Texture2D(ComPtr<ID3D12Device> device, UINT64 width, UINT height,
			DXGI_FORMAT dxgi_format, D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout,
			D3D12_RESOURCE_STATES state);
		DXGI_FORMAT GetDXGIFormat() const;
		UINT64 GetWidth() const;
		UINT GetHeight() const;
	private:
		DXGI_FORMAT dxgi_format_;
		UINT64 width_;
		UINT height_;
	};
}