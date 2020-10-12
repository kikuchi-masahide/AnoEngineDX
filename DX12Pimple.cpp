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

}

void DX12Pimple::CleanUp()
{
}

