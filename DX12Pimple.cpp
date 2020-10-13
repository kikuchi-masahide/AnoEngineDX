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

void DX12Pimple::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	if (FAILED(
		mDevice->CreateCommandQueue(
			&cmdQueueDesc, IID_PPV_ARGS(mCmdQueue.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12CommandQueue Initialization failed");
		throw 0;
	}
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
	//Direct3Dデバイス
	if (FAILED(
		D3D12CreateDevice(
			nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(mDevice.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12Device Initialization failed");
		throw 0;
	}
	//ファクトリの作成
#ifdef _DEBUG
	if (FAILED(
		CreateDXGIFactory2(
			DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(mFactory.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("IDXGIFactory6 Initialization failed");
		throw 0;
	}
#else
	if (FAILED(
		CreateDXGIFactory1(
			IID_PPV_ARGS(mFactory.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("IDXGIFactory6 Initialization failed");
		throw 0;
	}
#endif
	//コマンドアロケータ
	if (FAILED(
		mDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCmdAllocator.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12CommandAllocator Initialization failed");
		throw 0;
	}
	if (FAILED(
		mDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAllocator.Get(), nullptr,
			IID_PPV_ARGS(mCmdList.ReleaseAndGetAddressOf())
		)
	))
	{
		Log::OutputCritical("ID3D12GraphicsCommandList Initialization failed");
		throw 0;
	}
	try
	{
		CreateCommandQueue();
	}
	catch (...) {
		throw;
	}
}

void DX12Pimple::CleanUp()
{
}

unsigned int DX12Pimple::CreateSwapChain(HWND _hwnd, UINT _width, UINT _height)
{
	return mSwapChainManager.AddSwapChain(
		mFactory, mCmdQueue, _hwnd, _width, _height);
}
