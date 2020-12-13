#pragma once

#include "DX12CommonInclude.h"

class DX12Device;
class DX12CmdList;

class DX12RootSignature final :public boost::noncopyable {
public:
	DX12RootSignature(DX12Device* _device);
	ComPtr<ID3D12RootSignature> mRootSignature;
	void SetRootSignature(DX12CmdList* _list);
private:
};