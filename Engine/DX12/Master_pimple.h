//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "SwapChain.h"

namespace DX12 {
	/// <summary>
	/// DX12関連のオブジェクトのメモリプールの使用について考える
	/// </summary>
	class Master_pimple final
	{
	public:
		void Initialize();
		void ProcessCommands();
		void ResetCmdAllocator();
		void ExecuteCmdLists();
		void FenceWaitingInProccessCommands();
		std::shared_ptr<SwapChain> CreateSwapChain(HWND hwnd, UINT width, UINT height);
		void SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
		void ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b);
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain);
		void Flip(std::shared_ptr<SwapChain> swapchain);
		void CleanUp();
	private:
		void InitDebugLayer();
		void InitDevice();
		void InitFactory();
		void InitCmdAllocator_List_Queue();
		void InitDirectXTex();
		void InitDebugSetting();
		ComPtr<ID3D12CommandAllocator> cmd_allocator_;
		ComPtr<ID3D12GraphicsCommandList> cmd_list_;
		ComPtr<ID3D12CommandQueue> cmd_queue_;
		ComPtr<ID3D12Device> device_;
		ComPtr<ID3D12InfoQueue> info_queue_;
		ComPtr<IDXGIFactory6> factory_;
	};
}

