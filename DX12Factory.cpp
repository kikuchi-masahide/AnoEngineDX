#include "DX12Factory.h"

DX12Factory::DX12Factory()
	:mFactory()
{}

void DX12Factory::Initialize()
{
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

ComPtr<IDXGIFactory6> DX12Factory::GetFactory()
{
	return mFactory;
}

void DX12Factory::CleanUp()
{
	mFactory.Reset();
	{
		Log::OutputTrivial("DX12Factory::mFactory's refcount != 1");
	}
}
