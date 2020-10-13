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
	Log::Init();
	try {
		mdx12.Initialize();
	}
	catch (...) {
		assert(0);
	}
	mIsSceneChangable = true;
	return true;
}

/// <summary>
/// ゲームの終了処理
/// </summary>
void Game::Shutdown()
{
	mdx12.CleanUp();
}

Game::Game()
{
}

void Game::ChangeScene(boost::shared_ptr<Scene> _scene)
{
	if (!mIsSceneChangable) mPandingScene = _scene;
	else mCurrentScene = _scene;
}

boost::shared_ptr<Window> Game::GetWindow(int _windownum)
{
	if (_windownum >= mWindows.size())
	{
		Log::OutputCritical("Window Index out of range");
		assert(0);
	}
	return mWindows[_windownum];
}

unsigned int Game::AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height, LPCWSTR _windowTitle)
{
	boost::shared_ptr<Window> ptr(
		new Window(_wndproc,_classID,_width,_height,_windowTitle)
	);
	mWindows.push_back(ptr);
	return (unsigned int)mWindows.size() - 1;
}

/// <summary>
/// 入力処理
/// </summary>
void Game::ProcessInput()
{
}

/// <summary>
/// ゲーム全体の更新
/// </summary>
void Game::UpdateGame()
{
	mIsSceneChangable = false;
	mCurrentScene->Update();
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
		mCurrentScene = mPandingScene;
		mPandingScene.reset();
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
	for (int i = 0; i < mWindows.size(); i++)
	{
		mWindows[i]->ShowWindow();
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
	}
}

void Game::BeforeOutput()
{
}

bool Game::AfterOutput()
{
	return true;
}