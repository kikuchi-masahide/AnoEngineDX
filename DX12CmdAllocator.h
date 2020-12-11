#pragma once

#include "DX12CommonInclude.h"

class DX12Device;

class DX12CmdAllocator final :public boost::noncopyable {
public:
	DX12CmdAllocator();
	void Initialize(DX12Device* _device);
	ComPtr<ID3D12CommandAllocator> GetCmdAllocator();
	void CleanUp();
	void Reset();
private:
	ComPtr<ID3D12CommandAllocator> mCmdAllocator;
};