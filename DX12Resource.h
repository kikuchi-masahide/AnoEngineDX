#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"

class DX12Device;
class DX12CmdList;
class DX12DescriptorHeap;

class DX12Resource final :public boost::noncopyable {
public:
	DX12Resource(ComPtr<ID3D12Device> _device, DX12Config::ResourceHeapType _heaptype,UINT64 _width,UINT _height);
	DX12Resource(ComPtr<ID3D12Device> _device, DirectX::TexMetadata& _metadata);
	DX12Resource(ComPtr<IDXGISwapChain4> _swapchain,UINT _n);
	DX12Resource(ComPtr<ID3D12Device> _device, UINT64 _width, UINT64 _height, float _r, float _g, float _b, float _alpha);
	void* Map();
	void Unmap();
	//バッファの仮想アドレス
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	//コマンドリストに頂点バッファをセット
	void SetVertexBuffers(ComPtr<ID3D12GraphicsCommandList> _list,unsigned int _slotid, SIZE_T _allsize,
		SIZE_T _sizepervertex);
	//コマンドリストにインデックスバッファをセット
	void SetIndexBuffers(ComPtr<ID3D12GraphicsCommandList> _list, unsigned int _vertnum);
	ComPtr<ID3D12Resource> mResource;
	//リソースバリアのセット
	void SetResourceBarrier(ComPtr<ID3D12GraphicsCommandList> _list, DX12Config::ResourceBarrierState _before, DX12Config::ResourceBarrierState _after);
	/// <summary>
	/// 指定ディスクリプタヒープ上にこのリソースのレンダーターゲットビューを作る
	/// </summary>
	/// <param name="_n">ディスクリプタヒープの何番目にビューを作るか</param>
	void CreateRenderTargetView(ComPtr<ID3D12Device> _device, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n);
	/// <summary>
	/// 指定ディスクリプタヒープ上にこのリソースのレンダーターゲットビューを作る
	/// </summary>
	/// <param name="_n">ディスクリプタヒープの何番目にビューを作るか</param>
	void CreateShaderResourceView(ComPtr<ID3D12Device> _device, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n);
private:
	static D3D12_HEAP_TYPE mResourceHeapTypeCorrespond[(unsigned char)DX12Config::ResourceHeapType::size];
	static D3D12_RESOURCE_STATES mResourceStateCorrespond[(unsigned char)DX12Config::ResourceBarrierState::size];
};