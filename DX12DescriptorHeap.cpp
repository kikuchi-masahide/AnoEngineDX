#include "DX12DescriptorHeap.h"
#include "DX12Device.h"
#include "DX12Resource.h"
#include "DX12CmdList.h"
#include "DX12Pimple.h"

DX12DescriptorHeap::DX12DescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num, ComPtr<ID3D12Device> _device)
	:mNum(_num), mType(_type), mShaderVisibility(_vis)
{
	//mHeapDescStr‚Ì‘Î‰ž‰ÓŠ•ÏX
	mHeapDescStr.Type = mDescriptorHeapTypeCorrespond[(unsigned char)_type];
	mHeapDescStr.Flags = mShaderVisibilityCorrespond[(unsigned char)_vis];
	mHeapDescStr.NumDescriptors = _num;
	_device->CreateDescriptorHeap(&mHeapDescStr, IID_PPV_ARGS(mDescHeap.ReleaseAndGetAddressOf()));
	mDescriptorHandleSize = _device->GetDescriptorHandleIncrementSize(mDescriptorHeapTypeCorrespond[(unsigned char)_type]);
}

DX12DescriptorHeap::~DX12DescriptorHeap()
{
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetCPUDescriptorHandle(unsigned int _n)
{
	auto handle = mDescHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _n * mDescriptorHandleSize;
	return handle;
}

void DX12DescriptorHeap::SetDescriptorHeap(ComPtr<ID3D12GraphicsCommandList> _cmdlist)
{
	ID3D12DescriptorHeap* descHeapArray = { mDescHeap.Get() };
	_cmdlist->SetDescriptorHeaps(1,&descHeapArray);
}

const DX12Config::DescriptorHeapType DX12DescriptorHeap::GetDescriptorHeapType()
{
	return mType;
}

const DX12Config::ShaderVisibility DX12DescriptorHeap::GetShaderVisibility()
{
	return mShaderVisibility;
}

D3D12_GPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetGPUDescriptorHandle(unsigned int _n)
{
	auto handle = mDescHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _n * mDescriptorHandleSize;
	return handle;
}

boost::shared_ptr<DX12DescriptorHeap> DX12Pimple::CreateDescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num)
{
	return boost::shared_ptr<DX12DescriptorHeap>(
		new DX12DescriptorHeap(_type, _vis, _num, mDevice)
		);
}

D3D12_DESCRIPTOR_HEAP_DESC DX12DescriptorHeap::mHeapDescStr = {
	D3D12_DESCRIPTOR_HEAP_TYPE_RTV,0,D3D12_DESCRIPTOR_HEAP_FLAG_NONE,0
};

D3D12_DESCRIPTOR_HEAP_TYPE DX12DescriptorHeap::mDescriptorHeapTypeCorrespond[] = {
	/*RTV*/D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	/*CBV*/D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	/*SRV*/D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	/*UAV*/D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	/*DSV*/D3D12_DESCRIPTOR_HEAP_TYPE_DSV
};

D3D12_DESCRIPTOR_HEAP_FLAGS DX12DescriptorHeap::mShaderVisibilityCorrespond[] = {
	D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
};

void DX12Pimple::SetDescriptorHeap(boost::shared_ptr<DX12DescriptorHeap> _descHeap)
{
	_descHeap->SetDescriptorHeap(mCmdList);
}
