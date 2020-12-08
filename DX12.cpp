#include "DX12.h"
#include "DX12Device.h"
#include "DX12Factory.h"
#include "DX12CmdList.h"
#include "DX12CmdAllocator.h"
#include "DX12CmdQueue.h"
#include "SwapChainManager.h"

DX12::DX12()
	:mFactory(new DX12Factory()), mDevice(new DX12Device()), mCmdList(new DX12CmdList()),
	mCmdAllocator(new DX12CmdAllocator()), mCmdQueue(new DX12CmdQueue()),
	mSwapChainManager(new SwapChainManager())
{}

void DX12::Initialize()
{
	//デバッグレイヤの有効化
#ifdef _DEBUG
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
	delete mSwapChainManager;
	delete mFactory;
	delete mDevice;
	delete mCmdAllocator;
	delete mCmdList;
	delete mCmdQueue;
}

unsigned int DX12::CreateSwapChain(HWND _hwnd, UINT _width, UINT _height)
{
	return mSwapChainManager->AddSwapChain(mFactory, mCmdQueue, _hwnd, _width, _height);
}
