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
	//�����s���̂ݏ�����
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
	//�f�o�b�O���C���̗L����
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
		//Direct3D�f�o�C�X
		mDevice->Initialize();
		//�t�@�N�g���̍쐬
		mFactory->Initialize();
		//�R�}���h�A���P�[�^
		mCmdAllocator->Initialize(mDevice);
		//�R�}���h���X�g
		mCmdList->Initialize(mDevice, mCmdAllocator);
		//�R�}���h�L���[
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
	//���߂̃N���[�Y
	mCmdList->Close();
	//�R�}���h���X�g���s
	mCmdQueue->ExecuteCmdLists(mCmdList);
	//Reset�̑O�Ƀt�F���X���͂���
	FenceWaitingInProcessCommands();
	//�L���[���N���A
	mCmdAllocator->Reset();
	//�ĂуR�}���h���X�g�����߂鏀��
	mCmdList->Reset(mCmdAllocator);
	//�S�X���b�v�`�F�[���̃X���b�v
	mSwapChainManager->FlipAll();
}

void DX12::SetAndClearRenderTarget(unsigned int _id, float _r, float _g, float _b)
{
	mSwapChainManager->SetAndClearRenderTarget(_id, mCmdList, _r, _g, _b);
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
