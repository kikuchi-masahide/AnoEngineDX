#pragma once
#include "DX12.h"
#include "TextureManager.h"

class Scene;
class Window;

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();
	Game();
	/// <summary>
	/// ���t���[������ǂ̃V�[���Ɉڍs���邩�w�肷��D
	/// (���ۂɃV�[�����ς��͎̂��t���[�����炾���C���V�[���̏��������s����̂͌Ăяo�����Ȃ̂�����)
	/// </summary>
	template<class S, class... Args>
	void ChangeScene(Args... _args) {
		if (!mIsSceneChangable) {
			if (mPandingScene != nullptr)delete mPandingScene;
			mPandingScene = new S(this, _args...);
		}
		else {
			if (mCurrentScene != nullptr)delete mCurrentScene;
			mCurrentScene = new S(this, _args...);
		}
	};
	/// <summary>
	/// ���̃C���X�^���X��_windownum�Ԗڂ̃E�B���h�E��Ԃ�
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int _windownum);
	/// <summary>
	/// �E�B���h�E��Game�ɒǉ�����
	/// </summary>
	/// <returns>Game�ŉ��Ԗڂ̃E�B���h�E��(�E�B���h�E�̎w��ɂ͂��̔ԍ����g��)</returns>
	unsigned int AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height,
		LPCWSTR _windowTitle);
	//1�t���[��������̎���
	const double mTimeEps = (double)1000 / (double)60;
	//�O�̏����Ƃ̎��ԍ������̎��Ԃ𒴂����ꍇ���̒l�ɋ�������
	const double mProcessTimeLimit = mTimeEps * 3;
	DX12 mdx12;
	TextureManager mTexManager;
private:
	void ProcessInput();
	void UpdateGame();
	bool GenerateOutput();
	bool mIsRunning;
	Scene* mCurrentScene;
	Scene* mPandingScene;
	bool mIsSceneChangable;
	std::vector<boost::shared_ptr<Window>> mWindows;
	void BeforeOutput();
	bool AfterOutput();
	std::map<int, unsigned int> mWindowID2SwapChainID;
	std::vector<boost::shared_ptr<DX12SwapChain>> mSwapChains;
};