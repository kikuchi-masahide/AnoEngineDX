#include "DX12Resource.h"
#include "DX12Device.h"
#include "DX12CmdList.h"

DX12Resource::DX12Resource(DX12Device* _device, DX12Config::ResourceHeapType _heaptype, UINT64 _width, UINT _height)
{
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = mResourceHeapTypeCorrespond[(unsigned char)_heaptype];
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = _width;
	resourceDesc.Height = _height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	auto dev = _device->GetDevice();
	auto res = dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		throw 0;
		Log::OutputTrivial("Resource's initializing failed");
	}
}

DX12Resource::DX12Resource(DX12Device* _device, DirectX::TexMetadata& _metadata)
{
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//�e�N�X�`���p
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
	texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0
	D3D12_RESOURCE_DESC texResourceDesc = {};
	texResourceDesc.Format = _metadata.format;
	texResourceDesc.Width = _metadata.width;//��
	texResourceDesc.Height = _metadata.height;//����
	texResourceDesc.DepthOrArraySize = _metadata.arraySize;//2D�Ŕz��ł��Ȃ��̂łP
	texResourceDesc.MipLevels = _metadata.mipLevels;//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
	texResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(_metadata.dimension);//2D�e�N�X�`���p
	texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResourceDesc.SampleDesc.Count = 1;
	texResourceDesc.SampleDesc.Quality = 0;
	texResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	auto dev = _device->GetDevice();
	auto result = dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
		&texResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,//�R�s�[��
		nullptr,
		IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		throw 0;
		Log::OutputTrivial("Resource's initializing failed");
	}
}

void* DX12Resource::Map()
{
	void* map;
	mResource->Map(0, nullptr, &map);
	return map;
}

void DX12Resource::Unmap()
{
	mResource->Unmap(0,nullptr);
}

D3D12_GPU_VIRTUAL_ADDRESS DX12Resource::GetGPUVirtualAddress()
{
	return mResource->GetGPUVirtualAddress();
}

void DX12Resource::SetVertexBuffers(DX12CmdList* _list, unsigned int _slotid, SIZE_T _allsize, SIZE_T _sizepervertex)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	vbView.BufferLocation = mResource->GetGPUVirtualAddress();
	vbView.SizeInBytes = _allsize;
	vbView.StrideInBytes = _sizepervertex;
	_list->GetCmdList()->IASetVertexBuffers(_slotid, 1, &vbView);
}

void DX12Resource::SetIndexBuffers(DX12CmdList* _list, unsigned int _vertnum)
{
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ibView.BufferLocation = mResource->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeof(unsigned int) * _vertnum;
	_list->GetCmdList()->IASetIndexBuffer(&ibView);
}

D3D12_HEAP_TYPE DX12Resource::mResourceHeapTypeCorrespond[(unsigned char)DX12Config::ResourceHeapType::size] = {
		D3D12_HEAP_TYPE_DEFAULT,D3D12_HEAP_TYPE_UPLOAD,D3D12_HEAP_TYPE_READBACK
};