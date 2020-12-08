#include "DX12CmdAllocator.h"
#include "DX12Device.h"

DX12CmdAllocator::DX12CmdAllocator()
	:mCmdAllocator()
{}

void DX12CmdAllocator::Initialize(DX12Device* _device)
{
	if (FAILED(
		_device->GetDevice()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCmdAllocator.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12CommandAllocator Initialization failed");
		throw 0;
	}
}

ComPtr<ID3D12CommandAllocator> DX12CmdAllocator::GetCmdAllocator()
{
	return mCmdAllocator;
}

void DX12CmdAllocator::CleanUp()
{
	mCmdAllocator.Reset();
}