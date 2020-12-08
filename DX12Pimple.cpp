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
	//�f�o�b�O���C���̗L����
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
		//Direct3D�f�o�C�X
		mDevice.Initialize();
		//�t�@�N�g���̍쐬
		mFactory.Initialize();
		//�R�}���h�A���P�[�^
		mCmdAllocator.Initialize(mDevice);
		//�R�}���h���X�g
		mCmdList.Initialize(mDevice, mCmdAllocator);
		//�R�}���h�L���[
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
