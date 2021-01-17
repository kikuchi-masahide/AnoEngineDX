#include "DX12.h"
#include "DX12Device.h"
#include "DX12Factory.h"
#include "DX12CmdList.h"
#include "DX12CmdAllocator.h"
#include "DX12CmdQueue.h"
#include "SwapChainManager.h"
#include "DX12DescriptorHeap.h"
#include "DX12Resource.h"
#include "DX12ShaderObject.h"
#include "DX12GraphicsPipeline.h"
#include "DX12RootSignature.h"

void DX12::EnableDebugLayer()
{
	ComPtr<ID3D12Debug> debugLayer;
	if (FAILED(
		D3D12GetDebugInterface(
			IID_PPV_ARGS(debugLayer.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12Debug Initialization failed");
		throw 0;
	}
	debugLayer->EnableDebugLayer();
}

void DX12::FenceWaitingInProcessCommands()
{
	static ComPtr<ID3D12Fence> fence;
	static UINT64 fenceVal = 0;
	//初実行時のみ初期化
	if (!fence) {
		fence = mDevice->CreateFence(fenceVal);
	}
	mCmdQueue->Signal(fence, ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

DX12::DX12()
	:mFactory(new DX12Factory()), mDevice(new DX12Device()), mCmdList(new DX12CmdList()),
	mCmdAllocator(new DX12CmdAllocator()), mCmdQueue(new DX12CmdQueue()),
	mSwapChainManager(new SwapChainManager())
{}

void DX12::Initialize()
{
	//デバッグレイヤの有効化
#ifdef _DEBUG
	try {
		EnableDebugLayer();
	}
	catch (...)
	{
		throw;
	}
#endif
	try {
		//Direct3Dデバイス
		mDevice->Initialize();
		//ファクトリの作成
		mFactory->Initialize();
		//コマンドアロケータ
		mCmdAllocator->Initialize(mDevice);
		//コマンドリスト
		mCmdList->Initialize(mDevice, mCmdAllocator);
		//コマンドキュー
		mCmdQueue->Initialize(mDevice);
		//テクスチャロード用のCOM初期化
		if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
		{
			throw 0;
		}
	}
	catch (...)
	{
		throw;
	}
}

void DX12::CleanUp()
{
	mSwapChainManager->CleanUp();
	delete mSwapChainManager;
	mCmdQueue->CleanUp();
	delete mCmdQueue;
	mCmdList->CleanUp();
	delete mCmdList;
	mCmdAllocator->CleanUp();
	delete mCmdAllocator;
	mFactory->CleanUp();
	delete mFactory;
	mDevice->CleanUp();
	delete mDevice;
}

unsigned int DX12::CreateSwapChain(
	HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap
)
{
	return mSwapChainManager->AddSwapChain(mFactory, mCmdQueue, mDevice, _hwnd, _width, _height, _descheap);
}

boost::shared_ptr<DX12DescriptorHeap> DX12::CreateDescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num)
{
	return boost::shared_ptr<DX12DescriptorHeap>(
		new DX12DescriptorHeap(_type, _vis, _num, mDevice)
		);
}

void DX12::ProcessCommands()
{
	CloseRenderTarget();
	//命令のクローズ
	mCmdList->Close();
	//コマンドリスト実行
	mCmdQueue->ExecuteCmdLists(mCmdList);
	//Resetの前にフェンスをはさむ
	FenceWaitingInProcessCommands();
	//キューをクリア
	mCmdAllocator->Reset();
	//再びコマンドリストをためる準備
	mCmdList->Reset(mCmdAllocator);
	//全スワップチェーンのスワップ
	mSwapChainManager->FlipAll(mDevice);
}

boost::shared_ptr<DX12Resource> DX12::CreateVertexBuffer(UINT64 _width)
{
	return boost::shared_ptr<DX12Resource>(
		new DX12Resource(mDevice, DX12Config::ResourceHeapType::UPLOAD, _width, 1)
		);
}

void* DX12::Map(boost::shared_ptr<DX12Resource> _resource)
{
	return _resource->Map();
}

void DX12::Unmap(boost::shared_ptr<DX12Resource> _resource)
{
	_resource->Unmap();
}

boost::shared_ptr<DX12ShaderObject> DX12::LoadShader(LPCWSTR _filename, DX12Config::ShaderType _shaderType)
{
	return boost::shared_ptr<DX12ShaderObject>(
		new DX12ShaderObject(_filename,_shaderType)
		);
}

boost::shared_ptr<DX12GraphicsPipeline> DX12::CreateGraphicsPipeline(
	boost::shared_ptr<DX12ShaderObject> _vertexShader,
	boost::shared_ptr<DX12ShaderObject> _pixelShader, DX12VertexLayout& _vertexLayout,
	DX12Config::PrimitiveTopologyType _primitive, UINT _numrt,
	boost::shared_ptr<DX12RootSignature> _rootsignature)
{
	return boost::shared_ptr<DX12GraphicsPipeline>(
		new DX12GraphicsPipeline(
			mDevice, _vertexShader, _pixelShader, _vertexLayout,_primitive, _numrt,
			_rootsignature)
	);
}

boost::shared_ptr<DX12RootSignature> DX12::CreateRootSignature(DX12RootParameter& _rootparam)
{
	return boost::shared_ptr<DX12RootSignature>(
		new DX12RootSignature(mDevice,_rootparam)
		);
}

void DX12::SetGraphicsPipeline(boost::shared_ptr<DX12GraphicsPipeline> _pipeline)
{
	_pipeline->SetGraphicsPipeline(mCmdList);
}

void DX12::SetRootSignature(boost::shared_ptr<DX12RootSignature> _root)
{
	_root->SetRootSignature(mCmdList);
}

void DX12::SetPrimitiveTopology(DX12Config::PrimitiveTopologyType _prim)
{
	mCmdList->SetPrimitive(_prim);
}

void DX12::SetVertexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _slotid, SIZE_T _allsize, SIZE_T _sizepervertex)
{
	_resource->SetVertexBuffers(mCmdList, _slotid, _allsize, _sizepervertex);
}

void DX12::DrawInstanced(UINT _vertnum, UINT _instnum, UINT _vdataoffset, UINT _instoffset)
{
	mCmdList->DrawInstanced(_vertnum, _instnum, _vdataoffset, _instoffset);
}

void DX12::DrawIndexedInstanced(UINT _indexnumperinst, UINT _instnum, UINT _indoffset, UINT _vdataoffset, UINT _instoffset)
{
	mCmdList->DrawIndexedInstanced(_indexnumperinst, _instnum, _indoffset, _vdataoffset, _instoffset);
}

void DX12::SetViewports(UINT _widthpx, UINT _heightpx, int _topleftx, int _toplefty, float _maxdepth, float _mindepth)
{
	mCmdList->SetViewports(_widthpx, _heightpx, _topleftx, _toplefty, _maxdepth, _mindepth);
}

void DX12::SetScissorrect(float _top, float _bottom, float _left, float _right)
{
	mCmdList->SetScissorrect(_top, _bottom, _left, _right);
}

void DX12::SetRenderTarget(unsigned int _id)
{
	mSwapChainManager->OpenRenderTarget(_id,mCmdList);
}

void DX12::ClearRenderTarget(float _r, float _g, float _b)
{
	mSwapChainManager->ClearRenderTarget(mCmdList,_r, _g, _b);
}

void DX12::CloseRenderTarget()
{
	mSwapChainManager->CloseRenderTarget(mCmdList);
}

void DX12::SetIndexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _vertnum)
{
	_resource->SetIndexBuffers(mCmdList, _vertnum);
}

boost::shared_ptr<DX12Resource> DX12::CreateIndexBuffer(unsigned int _vertnum)
{
	return boost::shared_ptr<DX12Resource>(
		new DX12Resource(mDevice, DX12Config::ResourceHeapType::UPLOAD, sizeof(unsigned int) * _vertnum, 1)
		);
}

boost::shared_ptr<DX12Resource> DX12::CreateTextureUploadBuffer(unsigned int _rowpitch,unsigned int _height)
{
	auto aligned = _rowpitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - _rowpitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	return boost::shared_ptr<DX12Resource>(new DX12Resource(
		mDevice,DX12Config::ResourceHeapType::UPLOAD,aligned*_height,1
	));
}

boost::shared_ptr<DX12Resource> DX12::LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num)
{
	assert(
		_desc->GetDescriptorHeapType() == DX12Config::DescriptorHeapType::SRV &&
		_desc->GetShaderVisibility() == DX12Config::ShaderVisibility::SHADER_VISIBLE
	);
	auto dev = mDevice->GetDevice();
	//WICテクスチャのロード
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};
	auto result = LoadFromWICFile(_filename, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result))throw 0;
	auto img = scratchImg.GetImage(0, 0, 0);//生データ抽出

	auto alignmentedSize = img->rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - img->rowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	auto uploadResource = CreateTextureUploadBuffer(img->rowPitch,img->height);


	//読み取り用バッファ
	auto texResource = boost::shared_ptr<DX12Resource>(
		new DX12Resource(mDevice, metadata)
		);

	//転送用リソースにマップ
	uint8_t* mapforImg = (uint8_t*)uploadResource->Map();
	auto srcAddress = img->pixels;
	auto rowPitch = alignmentedSize;
	for (int y = 0; y < img->height; ++y) {
		std::copy_n(srcAddress,
			rowPitch, 
			mapforImg);//コピー
		//1行ごとの辻褄を合わせてやる
		srcAddress += img->rowPitch;
		mapforImg += rowPitch;
	}

	uploadResource->Unmap();//アンマップ

	auto texResourcerow = texResource->mResource.Get();
	auto uploadResourcerow = uploadResource->mResource.Get();


	//uploadからtexへのコピー
	D3D12_TEXTURE_COPY_LOCATION src = {}, dst = {};
	dst.pResource = texResourcerow;
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;
	src.pResource = uploadResourcerow;//中間バッファ
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;//フットプリント指定
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
	UINT nrow;
	UINT64 rowsize, size;
	auto desc = texResourcerow->GetDesc();
	dev->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
	src.PlacedFootprint = footprint;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = metadata.width;
	src.PlacedFootprint.Footprint.Height = metadata.height;
	src.PlacedFootprint.Footprint.Depth = metadata.depth;
	src.PlacedFootprint.Footprint.RowPitch = alignmentedSize;
	src.PlacedFootprint.Footprint.Format = img->format;
	auto commandlist = mCmdList->GetCmdList();
	commandlist->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

	//リソースバリア
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = dst.pResource;
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	commandlist->ResourceBarrier(1, &BarrierDesc);
	commandlist->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdlists[] = { commandlist.Get() };
	auto commandqueue = mCmdQueue->GetCmdQueue();
	commandqueue->ExecuteCommandLists(1, cmdlists);
	FenceWaitingInProcessCommands();
	mCmdAllocator->GetCmdAllocator()->Reset();//キューをクリア
	commandlist->Reset(mCmdAllocator->GetCmdAllocator().Get(), nullptr);

	//ディスクリプタ
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	dev->CreateShaderResourceView(texResourcerow, &srvDesc, _desc->GetCPUDescriptorHandle(_num));

	return texResource;
}

void DX12::SetDescriptorHeap(boost::shared_ptr<DX12DescriptorHeap> _descHeap)
{
	_descHeap->SetDescriptorHeap(mCmdList);
}

void DX12::SetGraphicsRootDescriptorTable(unsigned int _rootParamIndex, boost::shared_ptr<DX12DescriptorHeap> _descHeap, unsigned int _descHeapIndex)
{
	mCmdList->GetCmdList()->SetGraphicsRootDescriptorTable(_rootParamIndex, _descHeap->GetGPUDescriptorHandle(_descHeapIndex));
}
