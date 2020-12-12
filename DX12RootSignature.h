#pragma once

#include "DX12CommonInclude.h"

class DX12Device;

class DX12RootSignature final :public boost::noncopyable {
public:
	DX12RootSignature(DX12Device* _device);
	ComPtr<ID3D12RootSignature> mRootSignature;
private:
};