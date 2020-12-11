#pragma once

#include "DX12CommonInclude.h"

class DX12Device;
class DX12CmdList;

class DX12CmdQueue final:public boost::noncopyable{
public:
	DX12CmdQueue();
	void Initialize(DX12Device* _device);
	ComPtr<ID3D12CommandQueue> GetCmdQueue();
	void CleanUp();
	void ExecuteCmdLists(DX12CmdList* _list);
	void Signal(ComPtr<ID3D12Fence> _fence, UINT64 _value);
private:
	ComPtr<ID3D12CommandQueue> mCmdQueue;
};