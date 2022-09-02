//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class Texture2D;
	class DescriptorHeap;
	/// <summary>
	/// SwapChainñ{ëÃÇ‚backbufferÅARTVÇï€ë∂Ç∑ÇÈdescriptorheapÇÇ‹Ç∆ÇﬂÇƒä«óùÇ∑ÇÈ
	/// </summary>
	class SwapChain final:public boost::noncopyable{
	public:
		SwapChain(ComPtr<IDXGIFactory6> factory, ComPtr<ID3D12CommandQueue> cmdqueue,
			ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height);
		void SetDebugName(LPCWSTR debug_name);
		void Flip();
		UINT GetCurrentBackBufferIndex() const;
		std::shared_ptr<Texture2D> GetCurrentBackBuffer() const;
		std::shared_ptr<DescriptorHeap> GetDescriptorHeap() const;
	private:
		ComPtr<IDXGISwapChain4> swapchain_;
		std::shared_ptr<Texture2D> backbuffer_[2];
		std::shared_ptr<DescriptorHeap> desc_heap_;
	};
}