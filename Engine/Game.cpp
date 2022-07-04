//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Game.h"
#include "window.h"
#include "Scene.h"

#pragma comment(lib,"winmm.lib")

Game::Game()
	:tex_manager_(this), shader_manager_(this), input_system_(this), is_executing_destructor_(false)
{
	Log::OutputTrivial("Start Game::Initialize();");
	Log::Init();
	dx12_.Initialize();
	is_scene_changable_ = true;
	current_swapchain_id_ = -1;
}

Game::~Game()
{
	is_executing_destructor_ = true;
	DeleteScene(current_scene_);
	if (panding_scene_ != nullptr) {
		DeleteScene(panding_scene_);
	}
	dx12_.CleanUp();
}

void Game::AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height, LPCWSTR windowTitle, int windowid)
{
	BOOST_ASSERT_MSG(windowid >= 0, "windowID must be non-negative");
	BOOST_ASSERT_MSG(windows_.find(windowid) == windows_.end(), "windowID duplicating");
	//ウィンドウの追加
	boost::shared_ptr<Window> window(new Window(wndproc, classID, width, height, windowTitle));
	windows_[windowid] = window;
	//ウィンドウに付随するスワップチェーンの追加
	HWND hwnd = window->GetWindowHandle();
	boost::shared_ptr<DX12SwapChain> swapchain = dx12_.CreateSwapChain(hwnd, width, height);
	swapchains_.insert(std::pair<unsigned int, boost::shared_ptr<DX12SwapChain>>(windowid, swapchain));
	return;
}

boost::shared_ptr<Window> Game::GetWindow(int windowid) const
{
	auto itr = windows_.find(windowid);
	BOOST_ASSERT_MSG(itr != windows_.end(), "unregistered windowID");
	return itr->second;
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
			UpdateGame();
			millisec -= kFrameTimeDelta;
		}
		GenerateOutput();
		if (terminate_flag_) {
			for (auto window : windows_) {
				DestroyWindow(window.second->GetWindowHandle());
			}
			break;
		}
	}
}

void Game::OpenSwapChain(int windowid)
{
	auto itr = swapchains_.find(windowid);
	BOOST_ASSERT_MSG(itr != swapchains_.end(), "unregistered windowID");
	dx12_.OpenRenderTarget(swapchains_[windowid]);
	current_swapchain_id_ = windowid;
}

void Game::CloseSwapChain()
{
	if (current_swapchain_id_ != -1) {
		dx12_.CloseRenderTarget(swapchains_[current_swapchain_id_]);
		current_swapchain_id_ = -1;
	}
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
	//とりあえずレンダーターゲットのクリアのみ
	for (auto swapchain : swapchains_) {
		auto swapp = swapchain.second;
		dx12_.OpenRenderTarget(swapp);
		dx12_.ClearRenderTarget(swapp, 1.0f, 1.0f, 1.0f);
		dx12_.CloseRenderTarget(swapp);
	}
}

void Game::AfterOutput()
{
	//全てのスワップチェーンのフリップ
	for (auto itr = swapchains_.begin(); itr != swapchains_.end(); itr++) {
		dx12_.FlipSwapChain(itr->second);
	}
}

void Game::ProcessInput()
{
	input_system_.ProcessBeforeUpdate(this);
}

void Game::UpdateGame()
{
	BeforeUpdate();
	is_scene_changable_ = false;
	current_scene_->Update(&input_system_);
	AfterUpdate();
}

void Game::GenerateOutput()
{
	BeforeOutput();
	current_scene_->Output();
	AfterOutput();
	if (panding_scene_) {
		DeleteScene(current_scene_);
		//HACK:この代入で，mPandingSceneのcomponentなどが持っている参照の行き先がおかしくなっているらしい 何故?
		current_scene_ = panding_scene_;
		panding_scene_ = nullptr;
	}
	is_scene_changable_ = true;
}

void Game::DeleteScene(Scene* scene)
{
	scene->mDeleteCheck = true;
	delete scene;
}