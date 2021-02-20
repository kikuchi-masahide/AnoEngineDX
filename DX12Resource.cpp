#include "DX12Resource.h"
#include "DX12Pimple.h"
#include "DX12DescriptorHeap.h"

DX12Resource::DX12Resource(ComPtr<ID3D12Device> _device, DX12Config::ResourceHeapType _heaptype, UINT64 _width, UINT _height)
{
	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = mResourceHeapTypeCorrespond[(unsigned char)_heaptype];
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;
	//リソース設定
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
	auto res = _device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf()));
	if (FAILED(res)) {
		throw 0;
		Log::OutputTrivial("Resource's initializing failed");
	}
}

DX12Resource::DX12Resource(ComPtr<ID3D12Device> _device, DirectX::TexMetadata& _metadata)
{
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//テクスチャ用
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	texHeapProp.CreationNodeMask = 0;//単一アダプタのため0
	texHeapProp.VisibleNodeMask = 0;//単一アダプタのため0
	D3D12_RESOURCE_DESC texResourceDesc = {};
	texResourceDesc.Format = _metadata.format;
	texResourceDesc.Width = _metadata.width;//幅
	texResourceDesc.Height = _metadata.height;//高さ
	texResourceDesc.DepthOrArraySize = _metadata.arraySize;//2Dで配列でもないので１
	texResourceDesc.MipLevels = _metadata.mipLevels;//ミップマップしないのでミップ数は１つ
	texResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(_metadata.dimension);//2Dテクスチャ用
	texResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResourceDesc.SampleDesc.Count = 1;
	texResourceDesc.SampleDesc.Quality = 0;
	texResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	auto result = _device->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//特に指定なし
		&texResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,//コピー先
		nullptr,
		IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		throw 0;
		Log::OutputTrivial("Resource's initializing failed");
	}
}

DX12Resource::DX12Resource(ComPtr<IDXGISwapChain4> _swapchain, UINT _n)
{
	if (FAILED(
		_swapchain->GetBuffer(_n, IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf()))
	)) {
		Log::OutputCritical("mapping of backbuffers failed");
		throw 0;
	}
}

DX12Resource::DX12Resource(ComPtr<ID3D12Device> _device, UINT64 _width, UINT64 _height, float _r, float _g, float _b, float _alpha)
{
	//リソース設定はバックバッファとほぼ共通にする
	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resdesc.Width = _width;
	resdesc.Height = _height;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resdesc.SampleDesc.Count = 1;
	resdesc.SampleDesc.Quality = 0;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	float clsClr[4] = { _r,_g,_b,_alpha };
	auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clsClr);
	auto result = _device->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		Log::OutputCritical("Initialization of clear buffer failed\n");
		throw 0;
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

void DX12Resource::SetVertexBuffers(ComPtr<ID3D12GraphicsCommandList> _list, unsigned int _slotid, SIZE_T _allsize, SIZE_T _sizepervertex)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	vbView.BufferLocation = mResource->GetGPUVirtualAddress();
	vbView.SizeInBytes = _allsize;
	vbView.StrideInBytes = _sizepervertex;
	_list->IASetVertexBuffers(_slotid, 1, &vbView);
}

void DX12Resource::SetIndexBuffers(ComPtr<ID3D12GraphicsCommandList> _list, unsigned int _vertnum)
{
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ibView.BufferLocation = mResource->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeof(unsigned int) * _vertnum;
	_list->IASetIndexBuffer(&ibView);
}

void DX12Resource::SetResourceBarrier(ComPtr<ID3D12GraphicsCommandList> _list, DX12Config::ResourceBarrierState _before, DX12Config::ResourceBarrierState _after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = mResource.Get();
	barrier.Transition.StateBefore = mResourceStateCorrespond[(unsigned char)_before];
	barrier.Transition.StateAfter = mResourceStateCorrespond[(unsigned char)_after];
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	_list->ResourceBarrier(1, &barrier);
}

void DX12Resource::CreateRenderTargetView(ComPtr<ID3D12Device> _device, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n)
{
	assert(_descheap->GetDescriptorHeapType() == DX12Config::DescriptorHeapType::RTV);
	auto handle = _descheap->GetCPUDescriptorHandle(_n);
	_device->CreateRenderTargetView(mResource.Get(), nullptr, handle);
}

void DX12Resource::CreateShaderResourceView(ComPtr<ID3D12Device> _device, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n)
{
	assert(_descheap->GetDescriptorHeapType() == DX12Config::DescriptorHeapType::SRV);
	auto handle = _descheap->GetCPUDescriptorHandle(_n);
	//SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	_device->CreateShaderResourceView(mResource.Get(), &srvDesc, handle);
}

D3D12_HEAP_TYPE DX12Resource::mResourceHeapTypeCorrespond[(unsigned char)DX12Config::ResourceHeapType::size] = {
		D3D12_HEAP_TYPE_DEFAULT,D3D12_HEAP_TYPE_UPLOAD,D3D12_HEAP_TYPE_READBACK
};

D3D12_RESOURCE_STATES DX12Resource::mResourceStateCorrespond[(unsigned char)DX12Config::ResourceBarrierState::size] = {
	D3D12_RESOURCE_STATE_PRESENT,
	D3D12_RESOURCE_STATE_RENDER_TARGET,
	D3D12_RESOURCE_STATE_COPY_DEST,
	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
};


boost::shared_ptr<DX12Resource> DX12Pimple::CreateVertexBuffer(UINT64 _width)
{
	return boost::shared_ptr<DX12Resource>(
		new DX12Resource(mDevice, DX12Config::ResourceHeapType::UPLOAD, _width, 1)
		);
}

void* DX12Pimple::Map(boost::shared_ptr<DX12Resource> _resource)
{
	return _resource->Map();
}

void DX12Pimple::Unmap(boost::shared_ptr<DX12Resource> _resource) {
	return _resource->Unmap();
}

void DX12Pimple::SetVertexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _slotid,
	SIZE_T _allsize, SIZE_T _sizepervertex)
{
	_resource->SetVertexBuffers(mCmdList, _slotid, _allsize, _sizepervertex);
}

void DX12Pimple::SetIndexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _vertnum)
{
	_resource->SetIndexBuffers(mCmdList, _vertnum);
}

boost::shared_ptr<DX12Resource> DX12Pimple::CreateIndexBuffer(unsigned int _vertnum)
{
	return boost::shared_ptr<DX12Resource>(
		new DX12Resource(mDevice, DX12Config::ResourceHeapType::UPLOAD, sizeof(unsigned int) * _vertnum, 1)
		);
}

boost::shared_ptr<DX12Resource> DX12Pimple::CreateClearTexture(UINT64 _width, UINT64 _height, float _r, float _g, float _b, float _alpha)
{
	return boost::shared_ptr<DX12Resource>(new DX12Resource(
		mDevice, _width, _height, _r, _g, _b, _alpha
	));
}

void DX12Pimple::CreateRenderTargetView(boost::shared_ptr<DX12Resource> _resource, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n)
{
	_resource->CreateRenderTargetView(mDevice, _descheap, _n);
}

void DX12Pimple::CreateShaderResourceView(boost::shared_ptr<DX12Resource> _resource, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n)
{
	_resource->CreateShaderResourceView(mDevice, _descheap, _n);
}
