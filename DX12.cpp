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
	mSwapChainManager->FlipAll();
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

boost::shared_ptr<DX12RootSignature> DX12::CreateRootSignature()
{
	return boost::shared_ptr<DX12RootSignature>(
		new DX12RootSignature(mDevice)
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
