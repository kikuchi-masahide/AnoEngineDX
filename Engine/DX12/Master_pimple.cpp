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

std::shared_ptr<DX12::ShaderObject> DX12::Master_pimple::LoadShader(LPCWSTR filename, DX12::ShaderType shader_type)
{
	return std::make_shared<ShaderObject>(filename, shader_type);
}

std::shared_ptr<DX12::SwapChain> DX12::Master_pimple::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	//DBG_NEWのアロケータとstd::allocate_sharedを使うと「~SwapChainにアクセスできない」と言われるので。
	//効率は落ちるが、そもそもCreate~は頻繁に呼ぶものではないため妥協
	return std::shared_ptr<SwapChain>(DBG_NEW SwapChain(factory_, cmd_queue_, device_,
		hwnd, width, height));
}

std::shared_ptr<DX12::VertexBuffer> DX12::Master_pimple::CreateVertexBuffer(SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name)
{
	return std::make_shared<VertexBuffer>(device_, size_per_vert, vert_num, debug_name);
}

std::shared_ptr<DX12::IndexBuffer> DX12::Master_pimple::CreateIndexBuffer(int vert_num, LPCWSTR debug_name)
{
	return std::make_shared<DX12::IndexBuffer>(device_, vert_num, debug_name);
}

std::shared_ptr<DX12::DescriptorHeap> DX12::Master_pimple::CreateDescriptorHeap(int capacity, DescriptorHeapType type, DescriptorHeapShaderVisibility vis, LPCWSTR debug_name)
{
	return std::make_shared<DescriptorHeap>(device_, capacity, type, vis, debug_name);
}

std::shared_ptr<DX12::ConstBuffer> DX12::Master_pimple::CreateConstBuffer(SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name)
{
	return std::make_shared<ConstBuffer>(device_, size, heap_type, debug_name);
}

std::shared_ptr<DX12::DepthStencilBuffer> DX12::Master_pimple::CreateDepthStencilBuffer(UINT64 width, UINT64 height, ResourceHeapType heap_type)
{
	return std::make_shared<DepthStencilBuffer>(device_, width, height, heap_type);
}

void DX12::Master_pimple::CreateConstBufferView(std::shared_ptr<ConstBuffer> const_buff, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateConstBufferView(device_, const_buff, index);
}

void DX12::Master_pimple::CreateRenderTargetView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateRenderTargetView(device_, shader_resource, index);
}

void DX12::Master_pimple::CreateShaderResourceView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateShaderResourceView(device_, shader_resource, index);
}

void DX12::Master_pimple::CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	desc_heap->CreateDepthStencilBufferView(device_, dsbuffer, index);
}

std::shared_ptr<DX12::RootSignature> DX12::Master_pimple::CreateRootSignature(int root_param_num)
{
	return std::make_shared<RootSignature>(root_param_num);
}

std::shared_ptr<DX12::GraphicsPipeline> DX12::Master_pimple::CreateGraphicsPipeline(std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader, VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer, PrimitiveTopology topology, std::shared_ptr<RootSignature> root_signature, LPCWSTR name)
{
	return std::make_shared<GraphicsPipeline>(device_, vertex_shader, pixel_shader, layout_unit_array,
		layout_unit_num, dsbuffer, topology, root_signature, name);
}

std::shared_ptr<DX12::ShaderResource> DX12::Master_pimple::LoadTexture(LPCWSTR filename, LPCWSTR debug_name)
{
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};
	auto result = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));
	auto img = scratchImg.GetImage(0, 0, 0);
	auto upload_aligned = img->rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - img->rowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	std::shared_ptr<ShaderResource> upload = std::make_shared<ShaderResource>(device_,
		ResourceHeapType::UPLOAD, DXGI_FORMAT_UNKNOWN, D3D12_RESOURCE_DIMENSION_BUFFER, upload_aligned*img->height, 1,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE, D3D12_HEAP_FLAG_NONE,
		ResourceBarrierState::UPLOAD_GENERIC_READ, debug_name);
	auto alignmentedsize = img->rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - img->rowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	uint8_t* mapforimg = static_cast<uint8_t*>(upload->Map());
	auto srcAddress = img->pixels;
	auto rowPitch = alignmentedsize;
	for (int y = 0; y < img->height; y++) {
		std::copy_n(srcAddress, rowPitch, mapforimg);
		srcAddress += img->rowPitch;
		mapforimg += rowPitch;
	}
	upload->Unmap();
	//最終的なコピー先
	std::shared_ptr<ShaderResource> texresource = std::make_shared<ShaderResource>(device_,
		ResourceHeapType::DEFAULT, img->format, static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension),
		metadata.width, metadata.height, D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_NONE, D3D12_HEAP_FLAG_NONE, ResourceBarrierState::COPY_DEST,
		debug_name);
	D3D12_TEXTURE_COPY_LOCATION src = {}, dst = {};
	dst.pResource = texresource->GetRawPtr();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;
	src.pResource = upload->GetRawPtr();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.SubresourceIndex = 0;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
	UINT nrow;
	UINT64 rowsize, size;
	auto desc = texresource->GetDesc();
	device_->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
	src.PlacedFootprint = footprint;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = metadata.width;
	src.PlacedFootprint.Footprint.Height = metadata.height;
	src.PlacedFootprint.Footprint.Depth = metadata.depth;
	src.PlacedFootprint.Footprint.RowPitch = alignmentedsize;
	src.PlacedFootprint.Footprint.Format = img->format;
	cmd_list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	SetResourceBarrier(texresource, ResourceBarrierState::COPY_DEST, ResourceBarrierState::PIXEL_SHADER_RESOURCE);
	cmd_list_->Close();
	ID3D12CommandList* cmdlists[] = { cmd_list_.Get() };
	cmd_queue_->ExecuteCommandLists(1, cmdlists);
	FenceWaitingInProccessCommands();
	cmd_allocator_->Reset();
	cmd_list_->Reset(cmd_allocator_.Get(), nullptr);
	return texresource;
}

void DX12::Master_pimple::SetResourceBarrier(std::shared_ptr<SwapChain> swapchain, ResourceBarrierState before, ResourceBarrierState after)
{
	swapchain->SetResourceBarrier(cmd_list_, static_cast<D3D12_RESOURCE_STATES>(before),
		static_cast<D3D12_RESOURCE_STATES>(after));
}

void DX12::Master_pimple::SetResourceBarrier(std::shared_ptr<ShaderResource> shader_resource, ResourceBarrierState before, ResourceBarrierState after)
{
	static D3D12_RESOURCE_BARRIER barrier = {
	D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,D3D12_RESOURCE_BARRIER_FLAG_NONE ,{}
	};
	barrier.Transition = {
		shader_resource->GetRawPtr(),D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		static_cast<D3D12_RESOURCE_STATES>(before),static_cast<D3D12_RESOURCE_STATES>(after)
	};
	cmd_list_->ResourceBarrier(1, &barrier);
}

void DX12::Master_pimple::AddRootParameterAsDescriptorTable(std::shared_ptr<RootSignature> root_signature, std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis)
{
	root_signature->AddRootParameterAsDescriptorTable(ranges, vis);
}

void DX12::Master_pimple::Serialize(std::shared_ptr<RootSignature> root_signature)
{
	root_signature->Serialize(device_);
}

void* DX12::Master_pimple::Map(std::shared_ptr<VertexBuffer> vert_buffer)
{
	return vert_buffer->Map();
}

unsigned int* DX12::Master_pimple::Map(std::shared_ptr<IndexBuffer> index_buffer) {
	return index_buffer->Map();
}

void* DX12::Master_pimple::Map(std::shared_ptr<ConstBuffer> const_buffer)
{
	return const_buffer->Map();
}

void DX12::Master_pimple::Unmap(std::shared_ptr<VertexBuffer> vert_buffer)
{
	vert_buffer->Unmap();
}

void DX12::Master_pimple::Unmap(std::shared_ptr<IndexBuffer> index_buffer)
{
	index_buffer->Unmap();
}

void DX12::Master_pimple::Unmap(std::shared_ptr<ConstBuffer> const_buffer) {
	const_buffer->Unmap();
}

void DX12::Master_pimple::ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b)
{
	swapchain->ClearRenderTarget(cmd_list_, r, g, b);
}

void DX12::Master_pimple::ClearDepthStencilView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value)
{
	assert(desc_heap->heap_type_ == DescriptorHeapType::DSV);
	cmd_list_->ClearDepthStencilView(desc_heap->GetCPUDescriptorHandle(index), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		depth_value, 0.0f, 0, nullptr);
}

void DX12::Master_pimple::SetRenderTarget(std::shared_ptr<SwapChain> swapchain)
{
	swapchain->SetRenderTarget(cmd_list_);
}

void DX12::Master_pimple::SetRenderTarget(std::shared_ptr<SwapChain> swapchain, std::shared_ptr<DescriptorHeap> desc_heap, int index)
{
	swapchain->SetRenderTarget(cmd_list_, desc_heap, index);
}

void DX12::Master_pimple::SetVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer, int slot_id)
{
	vertex_buffer->SetVertexBuffer(cmd_list_, slot_id);
}

void DX12::Master_pimple::SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer)
{
	index_buffer->SetIndexBuffer(cmd_list_);
}

void DX12::Master_pimple::SetRootSignature(std::shared_ptr<RootSignature> root_signature)
{
	root_signature->SetRootSignature(cmd_list_);
}

void DX12::Master_pimple::SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap)
{
	ID3D12DescriptorHeap* arr = desc_heap->GetRawPtr();
	cmd_list_->SetDescriptorHeaps(1, &arr);
}

void DX12::Master_pimple::SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap, int base_desc_heap_index)
{
	cmd_list_->SetGraphicsRootDescriptorTable(root_param_index, desc_heap->GetGPUDescriptorHandle(base_desc_heap_index));
}

void DX12::Master_pimple::SetGraphicsPipeline(std::shared_ptr<GraphicsPipeline> pipeline)
{
	pipeline->SetGraphicsPipeline(cmd_list_);
}

void DX12::Master_pimple::SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth)
{
	CD3DX12_VIEWPORT viewport(top_left_x, top_left_y, width, height, min_depth, max_depth);
	cmd_list_->RSSetViewports(1, &viewport);
}

void DX12::Master_pimple::SetScissorRect(LONG top_left_x,LONG top_left_y,LONG bottom_right_x,LONG bottom_right_y)
{
	CD3DX12_RECT rect(top_left_x, top_left_y, bottom_right_x, bottom_right_y);
	cmd_list_->RSSetScissorRects(1,&rect);
}

void DX12::Master_pimple::DrawInstanced(int vertex_num, int start_vertex_loc)
{
	//HACK:今は1インスタンスの描画に特化
	cmd_list_->DrawInstanced(vertex_num, 1, start_vertex_loc, 0);
}

void DX12::Master_pimple::DrawIndexedInstanced(int index_num,int start_index_loc)
{
	cmd_list_->DrawIndexedInstanced(index_num, 1, start_index_loc, 0, 0);
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
