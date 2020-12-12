#include "DX12CmdQueue.h"
#include "DX12Device.h"
#include "DX12CmdList.h"

DX12CmdQueue::DX12CmdQueue()
	:mCmdQueue()
{}

void DX12CmdQueue::Initialize(DX12Device* _device)
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	if (FAILED(
		_device->GetDevice()->CreateCommandQueue(
			&cmdQueueDesc, IID_PPV_ARGS(mCmdQueue.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12CommandQueue Initialization failed");
		throw 0;
	}
}

ComPtr<ID3D12CommandQueue> DX12CmdQueue::GetCmdQueue()
{
	return mCmdQueue;
}

void DX12CmdQueue::CleanUp()
{
	mCmdQueue.Reset();
}

void DX12CmdQueue::ExecuteCmdLists(DX12CmdList* _list)
{
	ID3D12CommandList* cmdlists[] = { _list->GetCmdList().Get() };
	mCmdQueue->ExecuteCommandLists(1, cmdlists);
}

void DX12CmdQueue::Signal(ComPtr<ID3D12Fence> _fence, UINT64 _value)
{
	mCmdQueue->Signal(_fence.Get(), _value);
}