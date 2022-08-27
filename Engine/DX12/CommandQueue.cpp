//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "CommandQueue.h"

#include "GraphicsCommandList.h"
#include "Fence.h"
#include "Log.h"

DX12::CommandQueue::CommandQueue(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE cmdlist_type)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = cmdlist_type;
	auto res = device->CreateCommandQueue(&desc, IID_PPV_ARGS(cmd_queue_.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		Log::OutputCritical("Initialization of ID3D12CommandQueue failed");
		throw 0;
	}
}

void DX12::CommandQueue::SetDebugName(LPCWSTR debug_name)
{
#ifdef _DEBUG
	cmd_queue_->SetName(debug_name);
#endif
}

void DX12::CommandQueue::ExecuteCommandLists(const std::vector<std::shared_ptr<GraphicsCommandList>>& lists)
{
	ID3D12CommandList** cmdlists = DBG_NEW ID3D12CommandList * [lists.size()];
	for (int n = 0; n < lists.size(); n++) {
		cmdlists[n] = lists[n]->GetCommandListRawPtr();
	}
	cmd_queue_->ExecuteCommandLists(lists.size(), cmdlists);
	delete cmdlists;
}

void DX12::CommandQueue::Signal(std::shared_ptr<Fence> fence, UINT64 value)
{
	cmd_queue_->Signal(fence->GetRawPtr(), value);
}

void DX12::CommandQueue::Wait(std::shared_ptr<Fence> fence, UINT64 value)
{
	cmd_queue_->Wait(fence->GetRawPtr(), value);
}

ID3D12CommandQueue* DX12::CommandQueue::GetRawPtr() const
{
	return cmd_queue_.Get();
}

