//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Master.h"

#include "CommandQueue.h"
#include "Fence.h"
#include "GraphicsCommandList.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"
#include "Buffer.h"
#include "Texture2D.h"
#include "DepthStencilBuffer.h"
#include "DescriptorHeap.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "Log.h"

void DX12::Master::Initialize()
{
	try {
#ifdef _DEBUG
		InitDebugLayer();
#endif
		InitDevice();
		InitFactory();
		InitDirectXTex();
		InitDebugSetting();
	}
	catch (...) {
		throw;
	}
}

std::shared_ptr<DX12::CommandQueue> DX12::Master::CreateCommandQueue(D3D12_COMMAND_LIST_TYPE cmdlist_type)
{
	return std::make_shared<CommandQueue>(device_, cmdlist_type);
}

std::shared_ptr<DX12::Fence> DX12::Master::CreateFence(UINT64 value0)
{
	return std::make_shared<Fence>(device_, value0);
}

std::shared_ptr<DX12::GraphicsCommandList> DX12::Master::CreateGraphicsCommandList(
	D3D12_COMMAND_LIST_TYPE cmdlist_type)
{
	return std::make_shared<GraphicsCommandList>(device_, cmdlist_type);
}

std::shared_ptr<DX12::SwapChain> DX12::Master::CreateSwapChain(std::shared_ptr<CommandQueue> cmd_queue,
	HWND hwnd, UINT width, UINT height)
{
	return std::make_shared<SwapChain>(factory_, cmd_queue->GetRawPtr(), device_, hwnd, width, height);
}

std::shared_ptr<DX12::VertexBuffer> DX12::Master::CreateVertexBuffer(D3D12_HEAP_TYPE heap_type,
	D3D12_RESOURCE_STATES state, SIZE_T size_per_vert, int vert_num)
{
	return std::make_shared<VertexBuffer>(device_, heap_type, state, size_per_vert, vert_num);
}

std::shared_ptr<DX12::IndexBuffer> DX12::Master::CreateIndexBuffer(D3D12_HEAP_TYPE heap_type,
	D3D12_RESOURCE_STATES state, int vert_num)
{
	return std::make_shared<IndexBuffer>(device_, heap_type, state, vert_num);
}

std::shared_ptr<DX12::ConstBuffer> DX12::Master::CreateConstBuffer(D3D12_HEAP_TYPE heap_type, SIZE_T size, D3D12_RESOURCE_STATES state)
{
	return std::make_shared<ConstBuffer>(device_, heap_type, size, state);
}

std::shared_ptr<DX12::Buffer> DX12::Master::CreateBuffer(D3D12_HEAP_TYPE heap_type, SIZE_T size,
	D3D12_RESOURCE_STATES state)
{
	return std::make_shared<Buffer>(device_, heap_type, size, state);
}

std::shared_ptr<DX12::Texture2D> DX12::Master::CreateTexture2D(UINT64 width, UINT height,
	DXGI_FORMAT dxgi_format, D3D12_HEAP_TYPE heap_type, D3D12_TEXTURE_LAYOUT texture_layout,
	D3D12_RESOURCE_STATES state)
{
	return std::make_shared<Texture2D>(device_, width, height, dxgi_format, heap_type,
		texture_layout, state);
}

std::shared_ptr<DX12::DepthStencilBuffer> DX12::Master::CreateDepthStencilBuffer(UINT64 width, UINT height,
	D3D12_HEAP_TYPE heap_type)
{
	return std::make_shared<DepthStencilBuffer>(device_, width, height, heap_type);
}

std::shared_ptr<DX12::DescriptorHeap> DX12::Master::CreateDescriptorHeap(int capacity,
	D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS vis)
{
	return std::make_shared<DescriptorHeap>(device_, capacity, type, vis);
}

void DX12::Master::CreateConstBufferView(std::shared_ptr<ConstBuffer> buffer,
	std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateConstBufferView(device_, buffer, index);
}

void DX12::Master::CreateTexture2DView(std::shared_ptr<Texture2D> shader_resource,
	std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateTexture2DView(device_, shader_resource, index);
}

void DX12::Master::CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateDepthStencilBufferView(device_, dsbuffer, index);
}

void DX12::Master::CreateSampler(std::shared_ptr<DescriptorHeap> desc_heap, int index,
	D3D12_TEXTURE_ADDRESS_MODE address_u, D3D12_TEXTURE_ADDRESS_MODE address_v)
{
	desc_heap->CreateSampler(device_, index, address_u, address_v);
}

void DX12::Master::Serialize(std::shared_ptr<RootSignature> root_signature)
{
	root_signature->Serialize(device_);
}

std::shared_ptr<DX12::GraphicsPipeline> DX12::Master::CreateGraphicsPipeline(
	std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
	const std::vector<VertexLayoutUnit>& vertex_layout, bool dsbuffer,
	D3D_PRIMITIVE_TOPOLOGY primitive_topology, std::shared_ptr<RootSignature> root_signature)
{
	return std::make_shared<GraphicsPipeline>(device_, vertex_shader, pixel_shader,
		vertex_layout, dsbuffer, primitive_topology, root_signature);
}

void DX12::Master::InitDebugLayer()
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

void DX12::Master::InitDevice()
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
		Log::OutputCritical("Initialization of ID3D12Device failed");
		throw 0;
	}
}

void DX12::Master::InitFactory()
{
#ifdef _DEBUG
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

void DX12::Master::InitDirectXTex()
{
	if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED))) {
		Log::OutputCritical("Initialization of DirectXTex failed");
		throw 0;
	}
}

void DX12::Master::InitDebugSetting()
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
