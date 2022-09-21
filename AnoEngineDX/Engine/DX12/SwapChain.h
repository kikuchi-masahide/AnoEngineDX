//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DescriptorHeap.h"
#include "Texture2D.h"

namespace DX12 {
	class Texture2D;
	/// <summary>
	/// SwapChain本体やbackbuffer、RTVを保存するdescriptorheapをまとめて管理する
	/// </summary>
	class SwapChain final {
	public:
		SwapChain();
		SwapChain(ComPtr<IDXGIFactory6> factory, ComPtr<ID3D12CommandQueue> cmdqueue,
			ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height);
		void SetDebugName(LPCWSTR debug_name);
		void Flip();
		bool IsValid() const;
		UINT GetCurrentBackBufferIndex() const;
		Texture2D GetCurrentBackBuffer() const;
		const DescriptorHeap GetDescriptorHeap() const;
	private:
		ComPtr<IDXGISwapChain4> swapchain_;
		Texture2D backbuffer_[2];
		DescriptorHeap desc_heap_;
	};
}