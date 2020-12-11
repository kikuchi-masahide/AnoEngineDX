#pragma once

#include "DX12CommonInclude.h"

class DX12Device final:public boost::noncopyable {
public:
	DX12Device();
	//������
	void Initialize();
	ComPtr<ID3D12Device> GetDevice();
	void CleanUp();
	//�t�F���X�̍쐬
	ComPtr<ID3D12Fence> CreateFence(UINT64 _initialvalue);
private:
	ComPtr<ID3D12Device> mDevice;
};