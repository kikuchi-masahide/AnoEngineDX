#pragma once

#include "DX12CommonInclude.h"

class DX12Device;
class DX12CmdAllocator;

class DX12CmdList final :public boost::noncopyable {
public:
	DX12CmdList();
	void Initialize(DX12Device* _device, DX12CmdAllocator* _cmdalloc);
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	void CleanUp();
	void Close();
	void Reset(DX12CmdAllocator* _cmdallocator);
private:
	ComPtr<ID3D12GraphicsCommandList> mCmdList;
};