#include "DX12Pimple.h"
#include "DX12DescriptorHeap.h"
#include "DX12GraphicsPipeline.h"
#include "DX12Resource.h"
#include "DX12RootParameter.h"
#include "DX12RootSignature.h"
#include "DX12ShaderObject.h"
#include "DX12VertexLayoutUnit.h"
#include "SwapChainManager.h"

DX12Pimple::DX12Pimple() 
:mSwapChainManager(new SwapChainManager()){}

void DX12Pimple::Initialize() {
	//デバッグレイヤの有効化
#ifdef _DEBUG
	try {
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
	}
	catch (...)
	{
		throw;
	}
#endif
	try {
		//デバイス初期化
		{
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
		//ファクトリ初期化
		{
#ifdef _DEBUG
			auto result = CreateDXGIFactory2(
				DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(mFactory.ReleaseAndGetAddressOf()));
			if (FAILED(result))
			{
				Log::OutputCritical("IDXGIFactory6 Initialization failed");
				throw 0;
			}
#else
			if (FAILED(
				CreateDXGIFactory1(
					IID_PPV_ARGS(_factory.ReleaseAndGetAddressOf())
				)
			))
			{
				Log::OutputCritical("IDXGIFactory6 Initialization failed");
				throw 0;
			}
#endif
		}
		//コマンドアロケータ初期化
		{
			if (FAILED(
				mDevice->CreateCommandAllocator(
					D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCmdAllocator.ReleaseAndGetAddressOf())
				)
			))
			{
				Log::OutputCritical("ID3D12CommandAllocator Initialization failed");
				throw 0;
			}
		}
		//コマンドリスト初期化
		{
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
		}
		//コマンドキュー初期化
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
		//テクスチャロード用のCOM初期化
		{
			if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
			{
				throw 0;
			}
		}
	}
	catch (...)
	{
		throw;
	}
}

void DX12Pimple::CleanUp() {
	mSwapChainManager->CleanUp();
	delete mSwapChainManager;
}

unsigned int DX12Pimple::CreateSwapChain(
	HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap
)
{
	return mSwapChainManager->AddSwapChain(mFactory, mCmdQueue, mDevice, _hwnd, _width, _height, _descheap);
}

void DX12Pimple::ProcessCommands()
{
	mSwapChainManager->CloseRenderTarget(mCmdList);
	//命令のクローズ
	mCmdList->Close();
	//コマンドリスト実行
	ExecuteCmdLists();
	//Resetの前にフェンスをはさむ
	FenceWaitingInProcessCommands();
	//キューをクリア
	mCmdAllocator->Reset();
	//再びコマンドリストをためる準備
	ResetCmdAllocator();
	//全スワップチェーンのスワップ
	mSwapChainManager->FlipAll(mDevice);
}

void DX12Pimple::ResetCmdAllocator()
{
	mCmdList->Reset(mCmdAllocator.Get(), nullptr);
}

void DX12Pimple::ExecuteCmdLists()
{
	ID3D12CommandList* cmdlists[] = { mCmdList.Get() };
	mCmdQueue->ExecuteCommandLists(1, cmdlists);
}

void DX12Pimple::FenceWaitingInProcessCommands()
{
	static ComPtr<ID3D12Fence> fence = nullptr;
	static UINT64 fenceVal = 0;
	//初実行時のみ初期化
	if (!fence) {
		mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	}
	mCmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

boost::shared_ptr<DX12Resource> DX12Pimple::CreateTextureUploadBuffer(unsigned int _rowpitch,unsigned int _height)
{
	auto aligned = _rowpitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - _rowpitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	return boost::shared_ptr<DX12Resource>(new DX12Resource(
		mDevice,DX12Config::ResourceHeapType::UPLOAD,aligned*_height,1
	));
}

boost::shared_ptr<DX12Resource> DX12Pimple::LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc, unsigned int _num)
{
	assert(
		_desc->GetDescriptorHeapType() == DX12Config::DescriptorHeapType::SRV &&
		_desc->GetShaderVisibility() == DX12Config::ShaderVisibility::SHADER_VISIBLE
	);
	//WICテクスチャのロード
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};
	auto result = LoadFromWICFile(_filename, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result))throw 0;
	auto img = scratchImg.GetImage(0, 0, 0);//生データ抽出

	auto alignmentedSize = img->rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - img->rowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	auto uploadResource = CreateTextureUploadBuffer(img->rowPitch,img->height);


	//読み取り用バッファ
	auto texResource = boost::shared_ptr<DX12Resource>(
		new DX12Resource(mDevice, metadata)
		);

	//転送用リソースにマップ
	uint8_t* mapforImg = (uint8_t*)uploadResource->Map();
	auto srcAddress = img->pixels;
	auto rowPitch = alignmentedSize;
	for (int y = 0; y < img->height; ++y) {
		std::copy_n(srcAddress,
			rowPitch, 
			mapforImg);//コピー
		//1行ごとの辻褄を合わせてやる
		srcAddress += img->rowPitch;
		mapforImg += rowPitch;
	}

	uploadResource->Unmap();//アンマップ

	auto texResourcerow = texResource->mResource.Get();
	auto uploadResourcerow = uploadResource->mResource.Get();


	//uploadからtexへのコピー
	D3D12_TEXTURE_COPY_LOCATION src = {}, dst = {};
	dst.pResource = texResourcerow;
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;
	src.pResource = uploadResourcerow;//中間バッファ
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;//フットプリント指定
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
	UINT nrow;
	UINT64 rowsize, size;
	auto desc = texResourcerow->GetDesc();
	mDevice->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
	src.PlacedFootprint = footprint;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = metadata.width;
	src.PlacedFootprint.Footprint.Height = metadata.height;
	src.PlacedFootprint.Footprint.Depth = metadata.depth;
	src.PlacedFootprint.Footprint.RowPitch = alignmentedSize;
	src.PlacedFootprint.Footprint.Format = img->format;
	mCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

	//リソースバリア
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = dst.pResource;
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	mCmdList->ResourceBarrier(1, &BarrierDesc);
	mCmdList->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdlists[] = { mCmdList.Get() };
	mCmdQueue->ExecuteCommandLists(1, cmdlists);
	FenceWaitingInProcessCommands();
	mCmdAllocator->Reset();//キューをクリア
	mCmdList->Reset(mCmdAllocator.Get(), nullptr);

	//ディスクリプタ
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	mDevice->CreateShaderResourceView(texResourcerow, &srvDesc, _desc->GetCPUDescriptorHandle(_num));

	return texResource;
}
