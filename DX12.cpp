#include "DX12.h"
#include "DX12Device.h"
#include "DX12Factory.h"
#include "DX12CmdList.h"
#include "DX12CmdAllocator.h"
#include "DX12CmdQueue.h"
#include "SwapChainManager.h"
#include "DX12DescriptorHeap.h"

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