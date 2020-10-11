#include "Game.h"
#include "window.h"
//#include "Scene.h"

#pragma comment(lib,"winmm.lib")

/// <summary>
/// �Q�[���S�̂̏�����
/// </summary>
/// <returns>�������ۂ�</returns>
bool Game::Initialize()
{
	Log::Init();
	return true;
}

/// <summary>
/// �Q�[���̏I������
/// </summary>
void Game::Shutdown()
{
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

int Game::AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height, LPCWSTR _windowTitle)
{
	boost::shared_ptr<Window> ptr(
		new Window(_wndproc,_classID,_width,_height,_windowTitle)
	);
	mWindows.push_back(ptr);
	return mWindows.size() - 1;
}

/// <summary>
/// ���͏���
/// </summary>
void Game::ProcessInput()
{
}

/// <summary>
/// �Q�[���S�̂̍X�V
/// </summary>
void Game::UpdateGame()
{
	mIsSceneChangable = false;
	//mCurrentScene->Update(_dt);
}

/// <summary>
/// �o�͐���
/// </summary>
bool Game::GenerateOutput()
{
	//�T�u�V�X�e���̏o�͏���
	BeforeOutput();
	//mCurrentScene->Output(_dt);
	if (!AfterOutput())return false;
	if (mPandingScene) {
		mCurrentScene = mPandingScene;
		mPandingScene.reset();
	}
	mIsSceneChangable = true;
	return true;
}

/// <summary>
/// ���C�����[�v
/// </summary>
void Game::RunLoop()
{
	DWORD start = timeGetTime();
	MSG msg = {};
	//"������"���܂��Ă��鎞��
	double millisec = 0;
	//�E�B���h�E��\�����Ă���
	for (int i = 0; i < mWindows.size(); i++)
	{
		mWindows[i]->ShowWindow();
	}
	//���b�Z�[�W���[�v
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