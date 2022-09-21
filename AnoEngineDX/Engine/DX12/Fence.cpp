//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Fence.h"

DX12::Fence::Fence()
{
}

DX12::Fence::Fence(ComPtr<ID3D12Device> device, UINT64 value0)
{
	device->CreateFence(value0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence_.ReleaseAndGetAddressOf()));
}

void DX12::Fence::SetDebugName(LPCWSTR debug_name)
{
	fence_->SetName(debug_name);
}

void DX12::Fence::SetEventOnCompletion(UINT64 value, HANDLE event)
{
	fence_->SetEventOnCompletion(value, event);
}

void DX12::Fence::Signal(UINT64 value)
{
	fence_->Signal(value);
}

bool DX12::Fence::IsValid() const
{
	return fence_;
}

UINT64 DX12::Fence::GetCompletedValue() const
{
	return fence_->GetCompletedValue();
}

ID3D12Fence* DX12::Fence::GetRawPtr() const
{
	return fence_.Get();
}
