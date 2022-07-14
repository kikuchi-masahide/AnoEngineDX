//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Master_pimple.h"

#include "Log.h"

void DX12::Master_pimple::Initialize()
{
	try {
#ifdef _DEBUG
		InitDebugLayer();
#endif
		InitDevice();
		InitFactory();
		InitCmdAllocator_List_Queue();
		InitDirectXTex();
		InitDebugSetting();
	}
	catch (...) {
		throw;
	}
}

void DX12::Master_pimple::ProcessCommands()
{
	cmd_list_->Close();
	ExecuteCmdLists();
	FenceWaitingInProccessCommands();
	cmd_allocator_->Reset();
	ResetCmdAllocator();
}

void DX12::Master_pimple::ResetCmdAllocator()
{
	cmd_list_->Reset(cmd_allocator_.Get(), nullptr);
}

void DX12::Master_pimple::ExecuteCmdLists()
{
	ID3D12CommandList* cmdlists[] = { cmd_list_.Get() };
	cmd_queue_->ExecuteCommandLists(1, cmdlists);
}

void DX12::Master_pimple::FenceWaitingInProccessCommands()
{
	static ComPtr<ID3D12Fence> fence = nullptr;
	static UINT64 fenceVal = 0;
	static auto fence_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!fence) {
		device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	}
	fence->SetEventOnCompletion(++fenceVal, fence_event);
	cmd_queue_->Signal(fence.Get(), fenceVal);
	WaitForSingleObject(fence_event, INFINITE);
}

std::shared_ptr<DX12::SwapChain> DX12::Master_pimple::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	//DBG_NEWのアロケータとstd::allocate_sharedを使うと「~SwapChainにアクセスできない」と言われるので。
	//効率は落ちるが、そもそもCreate~は頻繁に呼ぶものではないため妥協
	return std::shared_ptr<SwapChain>(DBG_NEW SwapChain(factory_, cmd_queue_, device_,
		hwnd, width, height));
}

void DX12::Master_pimple::SetResourceBarrier(std::shared_ptr<SwapChain> swapchain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	swapchain->SetResourceBarrier(cmd_list_, before, after);
}

void DX12::Master_pimple::ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b)
{
	swapchain->ClearRenderTarget(cmd_list_, r, g, b);
}

void DX12::Master_pimple::SetRenderTarget(std::shared_ptr<SwapChain> swapchain)
{
	swapchain->SetRenderTarget(cmd_list_);
}

void DX12::Master_pimple::Flip(std::shared_ptr<SwapChain> swapchain)
{
	swapchain->Flip();
}

void DX12::Master_pimple::CleanUp()
{
	//CommandQueueをdeleteするときのOBJECT_DELETED_WHILE_STILL_IN_USE対策
	ProcessCommands();
}

void DX12::Master_pimple::InitDebugLayer()
{
	ComPtr<ID3D12Debug1> debug_layer;
	if (FAILED(
		D3D12GetDebugInterface(IID_PPV_ARGS(debug_layer.ReleaseAndGetAddressOf()))
	)) {
		Log::OutputCritical("initializing debug layer failed");
		throw 0;
	}
	debug_layer->EnableDebugLayer();
	debug_layer->SetEnableGPUBasedValidation(true);
}

void DX12::Master_pimple::InitDevice()
{
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	auto succeed_level = D3D_FEATURE_LEVEL_10_1;
	for (auto lv : levels) {
		if (SUCCEEDED(
			D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(device_.ReleaseAndGetAddressOf()))
		)) {
			succeed_level = lv;
			break;
		}
	}
	if (succeed_level == D3D_FEATURE_LEVEL_10_1) {
		//HACK:throwしても強制終了してくれるわけではないので、ここに強制終了処理を入れた方がいい?
		Log::OutputCritical("Initialization of ID3D12Device failed");
		throw 0;
	}
}

void DX12::Master_pimple::InitFactory()
{
#ifdef _DEBUG
	//HACK:Outputがfileとstdにできてない
	//C++の例外の勉強
	if (FAILED(CreateDXGIFactory2(
		DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(factory_.ReleaseAndGetAddressOf())
	))) {
		Log::OutputCritical("Initialization of IDXGIFactory2 failed");
		throw 0;
	}
#else
	if (FAILED(CreateDXGIFactory1(
		IID_PPV_ARGS(factory_.ReleaseAndGetAddressOf())
	))) {
		Log::OutputCritical("Initialization of IDXGIFactory6 failed");
		throw 0;
	}
#endif
}

void DX12::Master_pimple::InitCmdAllocator_List_Queue()
{
	if (FAILED(device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmd_allocator_.ReleaseAndGetAddressOf())
	))) {
		Log::OutputCritical("Initialization of ID3D12CommandAllocator failed");
		throw 0;
	}
	if (FAILED(device_->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmd_allocator_.Get(), nullptr,
		IID_PPV_ARGS(cmd_list_.ReleaseAndGetAddressOf())
	))) {
		Log::OutputCritical("Initialization of ID3D12GraphicsCommandList failed");
		throw 0;
	}
	D3D12_COMMAND_QUEUE_DESC cmd_queue_desc = {};
	cmd_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmd_queue_desc.NodeMask = 0;
	cmd_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmd_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	if (FAILED(device_->CreateCommandQueue(
		&cmd_queue_desc, IID_PPV_ARGS(cmd_queue_.ReleaseAndGetAddressOf())
	))) {
		Log::OutputCritical("Initialization of ID3D12CommandQueue failed");
		throw 0;
	}
}

void DX12::Master_pimple::InitDirectXTex()
{
	if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED))) {
		Log::OutputCritical("Initialization of DirectXTex failed");
		throw 0;
	}
}

void DX12::Master_pimple::InitDebugSetting()
{
#ifdef _DEBUG
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings1> dred_setting;
	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(dred_setting.ReleaseAndGetAddressOf())))) {
		Log::OutputTrivial("Initialization of ID3D12DeviceRemovedExtendedDataSettings1 failed");
		throw 0;
	}
	dred_setting->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	dred_setting->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	dred_setting->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	device_.As(&info_queue_);
	D3D12_MESSAGE_ID deny_ids[] = {
		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE
	};
	D3D12_MESSAGE_SEVERITY severities[] = {
		D3D12_MESSAGE_SEVERITY_INFO
	};
	D3D12_INFO_QUEUE_FILTER filter = {};
	filter.DenyList.NumIDs = _countof(deny_ids);
	filter.DenyList.pIDList = deny_ids;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	info_queue_->PushStorageFilter(&filter);
	//D3D12のエラー時に止めるようにする
	info_queue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
#endif
}
