#include "Game.h"
#include "window.h"
#include "Scene.h"

#pragma comment(lib,"winmm.lib")

/// <summary>
/// ゲーム全体の初期化
/// </summary>
/// <returns>成功か否か</returns>
bool Game::Initialize()
{
	Log::OutputTrivial("Start Game::Initialize();");
	Log::Init();
	mdx12.Initialize();
	mIsSceneChangable = true;
	mCurrentSwapChain = -1;
	return true;
}

/// <summary>
/// ゲームの終了処理
/// </summary>
void Game::Shutdown()
{
	is_executing_destructor_ = true;
	//シーンの削除
	DeleteScene(mCurrentScene);
	if (mPandingScene != nullptr)
	{
		DeleteScene(mPandingScene);
	}

	//DX12まわりのクリーンアップ
	mdx12.CleanUp();
}

Game::Game()
	:mTexManager(this),mShaderManager(this),mInputSystem(this),is_executing_destructor_(false)
{
}

boost::shared_ptr<Window> Game::GetWindow(unsigned int _windownum)
{
	//存在チェック
	auto itr = mWindows.find(_windownum);
	BOOST_ASSERT_MSG(itr != mWindows.end(), "unregistered windowID");
	return itr->second;
}

void Game::AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height, LPCWSTR _windowTitle,unsigned int _windowid)
{
	//ウィンドウIDの重複チェック
	BOOST_ASSERT_MSG(mWindows.find(_windowid) == mWindows.end(), "windowID duplicating");
	//ウィンドウの追加
	boost::shared_ptr<Window> window(new Window(_wndproc, _classID, _width, _height, _windowTitle));
	mWindows[_windowid] = window;
	//ウィンドウに付随するスワップチェーンの追加
	HWND hwnd = window->GetWindowHandle();
	boost::shared_ptr<DX12SwapChain> swapchain = mdx12.CreateSwapChain(hwnd, _width, _height);
	mSwapChains.insert(std::pair<unsigned int,boost::shared_ptr<DX12SwapChain>>(_windowid,swapchain));
	return;
}

void Game::OpenSwapChain(unsigned int _winnum)
{
	//存在チェック
	auto itr = mSwapChains.find(_winnum);
	BOOST_ASSERT_MSG(itr != mSwapChains.end(), "unregistered windowID");
	mdx12.OpenRenderTarget(mSwapChains[_winnum]);
	mCurrentSwapChain = _winnum;
}

void Game::CloseSwapChain()
{
	if (mCurrentSwapChain != -1)
	{
		mdx12.CloseRenderTarget(mSwapChains[mCurrentSwapChain]);
		mCurrentSwapChain = -1;
	}
}

//Game::~Game()
//{
//	Shutdown();
//}
//
void Game::Terminate()
{
	terminate_flag_ = true;
}

/// <summary>
/// 入力処理
/// </summary>
void Game::ProcessInput()
{
	mInputSystem.ProcessBeforeUpdate(this);
}

/// <summary>
/// ゲーム全体の更新
/// </summary>
void Game::UpdateGame()
{
	mIsSceneChangable = false;
	//HACK:今Sceneが持ってる一連のUpdate処理をGameに持ってきて，パフォーマンスプロファイルを分かりやすくした方がいいか?
	mCurrentScene->Update(&mInputSystem);
}

/// <summary>
/// 出力生成
/// </summary>
bool Game::GenerateOutput()
{
	//サブシステムの出力準備
	BeforeOutput();
	mCurrentScene->Output();
	if (!AfterOutput())return false;
	if (mPandingScene) {
		DeleteScene(mCurrentScene);
		//HACK:この代入で，mPandingSceneのcomponentなどが持っている参照の行き先がおかしくなっているらしい 何故?
		mCurrentScene = mPandingScene;
		mPandingScene = nullptr;
	}
	mIsSceneChangable = true;
	return true;
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
	for (auto itr = mWindows.begin(); itr != mWindows.end(); itr++)
	{
		itr->second->ShowWindow();
	}
	DWORD start = timeGetTime();
	//メッセージループ
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			break;
		}
		DWORD now = timeGetTime();
		double time = millisec + (double)(now - start);
		if (time < mTimeEps)
		{
			continue;
		}
		if (time > mProcessTimeLimit)
		{
			time = mProcessTimeLimit;
		}
		millisec = time;
		start = now;
		ProcessInput();
		while (millisec > mTimeEps)
		{
			UpdateGame();
			millisec -= mTimeEps;
		}
		if (!GenerateOutput())return;
		if (terminate_flag_)
		{
			for (auto window : mWindows)
			{
				DestroyWindow(window.second->GetWindowHandle());
			}
		}
	}
}

void Game::BeforeOutput()
{
	//とりあえずレンダーターゲットのクリアのみ
	for (auto swapchain : mSwapChains) {
		auto swapp = swapchain.second;
		mdx12.OpenRenderTarget(swapp);
		mdx12.ClearRenderTarget(swapp, 1.0f, 1.0f, 1.0f);
		mdx12.CloseRenderTarget(swapp);
	}
}

bool Game::AfterOutput()
{
	//全てのスワップチェーンのフリップ
	for (auto itr = mSwapChains.begin(); itr != mSwapChains.end(); itr++)
	{
		mdx12.FlipSwapChain(itr->second);
	}
	return true;
}

void Game::DeleteScene(Scene* _scene)
{
	_scene->mDeleteCheck = true;
	delete _scene;
}
