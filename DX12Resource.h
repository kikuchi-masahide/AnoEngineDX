#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"

class DX12Device;
class DX12CmdList;

class DX12Resource final :public boost::noncopyable {
public:
	DX12Resource(DX12Device* _device, DX12Config::ResourceHeapType _heaptype,UINT64 _width,UINT _height);
	void* Map();
	void Unmap();
	//�o�b�t�@�̉��z�A�h���X
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	//�R�}���h���X�g�ɒ��_�o�b�t�@���Z�b�g
	void SetVertexBuffers(DX12CmdList* _list,unsigned int _slotid, SIZE_T _allsize,
		SIZE_T _sizepervertex);
	//�R�}���h���X�g�ɃC���f�b�N�X�o�b�t�@���Z�b�g
	void SetIndexBuffers(DX12CmdList* _list, unsigned int _vertnum);
private:
	static D3D12_HEAP_TYPE mResourceHeapTypeCorrespond[(unsigned char)DX12Config::ResourceHeapType::size];
	ComPtr<ID3D12Resource> mResource;
};