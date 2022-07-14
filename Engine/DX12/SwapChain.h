//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"

namespace DX12 {
	/// <summary>
	/// バックバッファやそのRTVをまとめるクラス
	/// </summary>
	class SwapChain final
	{
	public:
		SwapChain(ComPtr<IDXGIFactory6> factory, ComPtr<ID3D12CommandQueue> cmdqueue,
			ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height);
		~SwapChain();
		UINT GetCurrentBackBufferIndex();
		/// <summary>
		/// 今のバックバッファにバリアを張る
		/// </summary>
		void SetResourceBarrier(ComPtr<ID3D12GraphicsCommandList> cmdlist,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
		/// <summary>
		/// コマンドリストに今のバックバッファのクリア命令を投げる
		/// </summary>
		void ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> list, float r, float g, float b);
		/// <summary>
		/// RenderTargetに指定する
		/// </summary>
		void SetRenderTarget(ComPtr<ID3D12GraphicsCommandList> cmdlist);
		void Flip();
	private:
		ComPtr<IDXGISwapChain4> swapchain_;
		ComPtr<ID3D12Resource> buffer_[2];
		ComPtr<ID3D12DescriptorHeap> desc_heap_;
		UINT desc_handle_incr_size_;
	};
}

