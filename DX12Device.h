#pragma once

#include "DX12CommonInclude.h"

class DX12Device final:public boost::noncopyable {
public:
	DX12Device();
	//初期化
	void Initialize();
	ComPtr<ID3D12Device> GetDevice();
	void CleanUp();
	//フェンスの作成
	ComPtr<ID3D12Fence> CreateFence(UINT64 _initialvalue);
private:
	ComPtr<ID3D12Device> mDevice;
};