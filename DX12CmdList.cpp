#include "DX12CmdList.h"
#include "DX12Device.h"
#include "DX12CmdAllocator.h"

DX12CmdList::DX12CmdList()
	:mCmdList()
{}

void DX12CmdList::Initialize(DX12Device* _device, DX12CmdAllocator* _cmdalloc)
{
	if (FAILED(
		_device->GetDevice()->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdalloc->GetCmdAllocator().Get(), nullptr,
			IID_PPV_ARGS(mCmdList.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12GraphicsCommandList Initialization failed");
		throw 0;
	}
}

ComPtr<ID3D12GraphicsCommandList> DX12CmdList::GetCmdList()
{
	return mCmdList;
}

void DX12CmdList::CleanUp() {
	if (mCmdList.Reset() != 0)
	{
		Log::OutputTrivial("DX12CmdList::mCmdList's refcount != 1");
	}
}