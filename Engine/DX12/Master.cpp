//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Master.h"

#include "ConfigEnums.h"
#include "Master_pimple.h"

namespace {
	D3D12_RESOURCE_STATES ResourceBarrierStateConv[(char)DX12::ResourceBarrierState::size] = {
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	};
}

void DX12::Master::Initialize() {
	pimple_ = DBG_NEW Master_pimple;
	pimple_->Initialize();
}

void DX12::Master::ProcessCommands()
{
	pimple_->ProcessCommands();
}

std::shared_ptr<DX12::SwapChain> DX12::Master::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	return pimple_->CreateSwapChain(hwnd, width, height);
}

void DX12::Master::SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
	ResourceBarrierState before, ResourceBarrierState after)
{
	pimple_->SetResourceBarrier(swapchain, ResourceBarrierStateConv[(char)before],
		ResourceBarrierStateConv[(char)after]);
}

void DX12::Master::ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b)
{
	pimple_->ClearRenderTarget(swapchain, r, g, b);
}

void DX12::Master::SetRenderTarget(std::shared_ptr<SwapChain> swapchain)
{
	pimple_->SetRenderTarget(swapchain);
}

void DX12::Master::Flip(std::shared_ptr<SwapChain> swapchain)
{
	pimple_->Flip(swapchain);
}

void DX12::Master::CleanUp()
{
	pimple_->CleanUp();
	delete pimple_;
}
