#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"

class DX12Device;

class DX12Resource final :public boost::noncopyable {
public:
	DX12Resource(DX12Device* _device, DX12Config::ResourceHeapType _heaptype,UINT64 _width,UINT _height);
	void* Map();
	void Unmap();
	//バッファの仮想アドレス
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
private:
	static D3D12_HEAP_TYPE mResourceHeapTypeCorrespond[(unsigned char)DX12Config::ResourceHeapType::size];
	ComPtr<ID3D12Resource> mResource;
};