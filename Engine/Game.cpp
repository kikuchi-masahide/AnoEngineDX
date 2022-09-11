//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Game.h"
#include "window.h"
#include "Scene.h"
#include "GameObject.h"
#include "boost/pool/pool_alloc.hpp"

#pragma comment(lib,"winmm.lib")

Game::Game()
	:is_executing_destructor_(false)
{
	Log::Init();
	Log::OutputTrivial("DX12 Initialization");
	dx12_.Initialize();
	fontmaster_.Init(&dx12_);
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
	Log::OutputTrivial("Game::~Game()");
	Log::CleanUp();
}

std::weak_ptr<Window> Game::AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height,
	LPCWSTR windowTitle, int windowid)
{
	BOOST_ASSERT_MSG(windows_.find(windowid) == windows_.end(), "windowID duplicating");
	auto itr = windows_.emplace(windowid,
		std::make_shared<Window>(wndproc, classID, width, height, windowTitle)).first;
	return itr->second;
}

std::shared_ptr<Window> Game::GetWindow(int windowid) const
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
		while (millisec > kFrameTimeDelta) {
			int obj_num = 0;
			int update_comp_num = 0;
			int output_comp_num = 0;
			if (current_scene_) {
				obj_num = current_scene_->GetGameObjectNumber();
				update_comp_num = current_scene_->GetUpdateComponentNumber();
				output_comp_num = current_scene_->GetOutputComponentNumber();
			}
			ProcessInput();
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
	//すべてのswapchainのflip
	for (auto itr = windows_.begin(); itr != windows_.end(); itr++) {
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