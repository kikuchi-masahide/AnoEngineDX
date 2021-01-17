#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"

class DX12Device;
class DX12CmdList;

class DX12Resource final :public boost::noncopyable {
public:
	DX12Resource(ComPtr<ID3D12Device> _device, DX12Config::ResourceHeapType _heaptype,UINT64 _width,UINT _height);
	DX12Resource(ComPtr<ID3D12Device> _device, DirectX::TexMetadata& _metadata);
	void* Map();
	void Unmap();
	//�o�b�t�@�̉��z�A�h���X
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	//�R�}���h���X�g�ɒ��_�o�b�t�@���Z�b�g
	void SetVertexBuffers(ComPtr<ID3D12GraphicsCommandList> _list,unsigned int _slotid, SIZE_T _allsize,
		SIZE_T _sizepervertex);
	//�R�}���h���X�g�ɃC���f�b�N�X�o�b�t�@���Z�b�g
	void SetIndexBuffers(ComPtr<ID3D12GraphicsCommandList> _list, unsigned int _vertnum);
	ComPtr<ID3D12Resource> mResource;
private:
	static D3D12_HEAP_TYPE mResourceHeapTypeCorrespond[(unsigned char)DX12Config::ResourceHeapType::size];
};