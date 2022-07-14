//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "ConfigEnums.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace DX12 {
	class Master_pimple;
	class SwapChain;
	class Master final
	{
	public:
		void Initialize();
		void ProcessCommands();
		std::shared_ptr<SwapChain> CreateSwapChain(HWND hwnd, UINT width, UINT height);
		/// <summary>
		/// swapchainの今のバックバッファにバリアを張る
		/// </summary>
		void SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
			ResourceBarrierState before, ResourceBarrierState after);
		/// <summary>
		/// コマンドリストに今のバックバッファのクリア命令を投げる
		/// </summary>
		void ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b);
		/// <summary>
		/// swapchainの今のバックバッファをレンダーターゲットに指定する
		/// </summary>
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain);
		/// <summary>
		/// このswapchainをflipする
		/// </summary>
		void Flip(std::shared_ptr<SwapChain> swapchain);
		void CleanUp();
	private:
		Master_pimple* pimple_;
	};
}

