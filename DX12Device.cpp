#include "DX12Device.h"

DX12Device::DX12Device() :mDevice()
{}

void DX12Device::Initialize()
{
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
}

ComPtr<ID3D12Device> DX12Device::GetDevice()
{
	return mDevice;
}

void DX12Device::CleanUp()
{
	if (mDevice.Reset() != 0)
	{
		Log::OutputCritical("DX12Device::mDevice's refcount != 1");
	}
}
