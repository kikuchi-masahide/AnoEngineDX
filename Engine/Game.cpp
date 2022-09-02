//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Game.h"
#include "window.h"
#include "Scene.h"
#include "GameObject.h"
#include "boost/pool/pool_alloc.hpp"
#include "DX12/CommandQueue.h"
#include "DX12/SwapChain.h"
#include "DX12/Fence.h"
#include "DX12/GraphicsCommandList.h"
#include "DX12/Texture2D.h"

#pragma comment(lib,"winmm.lib")

Game::Game()
	:is_executing_destructor_(false)
{
	Log::Init();
	Log::OutputTrivial("DX12 Initialization");
	dx12_.Initialize();
	graphics_cmd_queue_ = dx12_.CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	graphics_cmd_queue_->SetDebugName(L"graphics_cmd_queue_");
	current_swapchain_id_ = -1;
	Scene::InitMemory();
}

Game::~Game()
{
	is_executing_destructor_ = true;
	if (current_scene_) {
		DeleteScene(current_scene_);
	}
	if (panding_scene_) {
		DeleteScene(panding_scene_);
	}
	boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(ComponentHandle<Component>)>::purge_memory();
	//OBJECT_DELETED_WHILE_STILL_IN_USE対策
	std::shared_ptr<DX12::Fence> fence = dx12_.CreateFence();
	auto eventhandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	fence->SetEventOnCompletion(1, eventhandle);
	graphics_cmd_queue_->Signal(fence, 1);
	WaitForSingleObject(eventhandle, INFINITE);
	Log::OutputTrivial("Game::~Game()");
	Log::CleanUp();
}

void Game::AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height, LPCWSTR windowTitle, int windowid, bool use_swapchain)
{
	BOOST_ASSERT_MSG(windowid >= 0, "windowID must be non-negative");
	BOOST_ASSERT_MSG(windows_.find(windowid) == windows_.end(), "windowID duplicating");
	//ウィンドウの追加
	boost::shared_ptr<Window> window(new Window(wndproc, classID, width, height, windowTitle));
	windows_[windowid] = window;
	if (use_swapchain) {
		//ウィンドウに付随するスワップチェーンの追加
		HWND hwnd = window->GetWindowHandle();
		std::shared_ptr<DX12::SwapChain> swapchain = dx12_.CreateSwapChain(graphics_cmd_queue_, hwnd, width, height);
		swapchain->SetDebugName(L"swapchain");
		swapchains_.emplace(windowid, swapchain);
		std::shared_ptr<DX12::DepthStencilBuffer> dsbuffer = dx12_.CreateDepthStencilBuffer(width, height);
		std::shared_ptr<DX12::DescriptorHeap> descheap = dx12_.CreateDescriptorHeap(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		dx12_.CreateDepthStencilBufferView(dsbuffer, descheap, 0);
		dsbuffers_.emplace(windowid, dsbuffer);
		dsv_descheaps_.emplace(windowid, descheap);
	}
	return;
}

boost::shared_ptr<Window> Game::GetWindow(int windowid) const
{
	auto itr = windows_.find(windowid);
	BOOST_ASSERT_MSG(itr != windows_.end(), "unregistered windowID");
	return itr->second;
}

void Game::SetRenderTarget(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid)
{
	auto itr = swapchains_.find(windowid);
	assert(itr != swapchains_.end());
	auto swapchain = itr->second;
	auto bbindex = swapchain->GetCurrentBackBufferIndex();
	auto rtv_desc = swapchain->GetDescriptorHeap();
	auto srv_desc = dsv_descheaps_.find(windowid)->second;
	cmd_list->SetRenderTargets(rtv_desc, bbindex, 1, srv_desc, 0);
}

void Game::SetBackbufferStateBarrier(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid,
	D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	auto itr = swapchains_.find(windowid);
	assert(itr != swapchains_.end());
	auto backbuffer = itr->second->GetCurrentBackBuffer();
	cmd_list->SetResourceBarrier(DX12::ResourceBarrierUnit(backbuffer, before, after));
}

void Game::ClearBackbuffer(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid,
	float r, float g, float b)
{
	auto itr = swapchains_.find(windowid);
	assert(itr != swapchains_.end());
	auto bbindex = itr->second->GetCurrentBackBufferIndex();
	auto rtv_desc = itr->second->GetDescriptorHeap();
	cmd_list->ClearRenderTargetView(rtv_desc, bbindex, r, g, b);
}

void Game::ClearDepthStencilBuffer(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid,
	float value)
{
	auto itr = dsv_descheaps_.find(windowid);
	assert(itr != dsv_descheaps_.end());
	cmd_list->ClearDepthStencilBufferView(itr->second, 0, value);
}

std::shared_ptr<DX12::SwapChain> Game::GetSwapChain(int windowid) const
{
	auto itr = windows_.find(windowid);
	if (itr != windows_.end()) {
		return swapchains_.find(windowid)->second;
	}
	else {
		return nullptr;
	}
}

std::shared_ptr<DX12::CommandQueue> Game::GetCommandQueue() const
{
	return graphics_cmd_queue_;
}

/// <summary>
/// メインループ
/// </summary>
void Game::RunLoop()
{
	MSG msg = {};
	//"微妙に"たまっている時間
	double millisec = 0;
	//ウィンドウを表示していく
	for (auto itr = windows_.begin(); itr != windows_.end(); itr++) {
		itr->second->ShowWindow();
	}
	DWORD start = timeGetTime();
	//メッセージループ
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		ProcessPandingScene();
		DWORD now = timeGetTime();
		double time = millisec + (double)(now - start);
		if (time < kFrameTimeDelta) {
			continue;
		}
		if (time > kProcessTimeDeltaLimit) {
			time = kProcessTimeDeltaLimit;
		}
		millisec = time;
		start = now;
		ProcessInput();
		while (millisec > kFrameTimeDelta) {
			int obj_num = 0;
			int update_comp_num = 0;
			int output_comp_num = 0;
			if (current_scene_) {
				obj_num = current_scene_->GetGameObjectNumber();
				update_comp_num = current_scene_->GetUpdateComponentNumber();
				output_comp_num = current_scene_->GetOutputComponentNumber();
			}
			DWORD update_time = timeGetTime();
			UpdateGame();
			update_time = timeGetTime() - update_time;
			DWORD output_time = timeGetTime();
			GenerateOutput();
			output_time = timeGetTime() - output_time;
			Log::UpdateDatas(update_time, output_time, obj_num, update_comp_num, output_comp_num);
			millisec -= kFrameTimeDelta;
		}
		if (terminate_flag_) {
			for (auto window : windows_) {
				DestroyWindow(window.second->GetWindowHandle());
			}
			break;
		}
	}
	//現在実行中の場合中断を通告
	async_initing_thread_.interrupt();
	async_initing_thread_.join();
}

void Game::Terminate()
{
	terminate_flag_ = true;
}

void Game::BeforeUpdate()
{
}

void Game::AfterUpdate()
{
}

void Game::BeforeOutput()
{
}

void Game::AfterOutput()
{
	//全てのスワップチェーンのフリップ
	for (auto itr = swapchains_.begin(); itr != swapchains_.end(); itr++) {
		itr->second->Flip();
	}
}

void Game::ProcessInput()
{
	input_system_.ProcessBeforeUpdate();
}

void Game::UpdateGame()
{
	BeforeUpdate();
	if (current_scene_) {
		current_scene_->Update(&input_system_);
	}
	AfterUpdate();
}

void Game::GenerateOutput()
{
	BeforeOutput();
	if (current_scene_) {
		current_scene_->Output();
	}
	AfterOutput();
}

void Game::AsyncInitializeScene(Scene* scene)
{
	try {
		scene->AsyncInitialize();
	}
	//アプリ終了、別のChangeSceneなどでスレッドの中断が入った場合
	catch (boost::thread_interrupted) {
		DeleteScene(scene);
		return;
	}
	{
		boost::unique_lock<boost::mutex> lock(panding_scene_mutex_);
		panding_scene_ = scene;
	}
}

void Game::ProcessPandingScene()
{
	boost::unique_lock<boost::mutex> lock(panding_scene_mutex_);
	if (panding_scene_) {
		if (current_scene_) {
			DeleteScene(current_scene_);
		}
		//HACK:この代入で，mPandingSceneのcomponentなどが持っている参照の行き先がおかしくなっているらしい 何故?
		current_scene_ = panding_scene_;
		panding_scene_ = nullptr;
	}
}

void Game::DeleteScene(Scene* scene)
{
	scene->delete_check_ = true;
	delete scene;
}