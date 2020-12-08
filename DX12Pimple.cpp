#include "DX12Pimple.h"

void DX12Pimple::EnableDebugLayer()
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

DX12Pimple::DX12Pimple() {}

void DX12Pimple::Initialize()
{
	//デバッグレイヤの有効化
#ifdef _DEBUG
	try {
		EnableDebugLayer();
	}
	catch (...)
	{
		throw 0;
	}
#endif
	try {
		//Direct3Dデバイス
		mDevice.Initialize();
		//ファクトリの作成
		mFactory.Initialize();
		//コマンドアロケータ
		mCmdAllocator.Initialize(mDevice);
		//コマンドリスト
		mCmdList.Initialize(mDevice, mCmdAllocator);
		//コマンドキュー
		mCmdQueue.Initialize(mDevice);
	}
	catch (...)
	{
		throw;
	}
}

void DX12Pimple::CleanUp()
{
}

unsigned int DX12Pimple::CreateSwapChain(HWND _hwnd, UINT _width, UINT _height)
{
	return mSwapChainManager.AddSwapChain(mFactory, mCmdQueue, _hwnd, _width, _height);
}
