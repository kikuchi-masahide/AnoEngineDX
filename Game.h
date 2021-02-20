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
	/// 次フレームからどのシーンに移行するか指定する．
	/// (実際にシーンが変わるのは次フレームからだが，次シーンの初期化が行われるのは呼び出し時なのが困る)
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
	/// このインスタンスで_windownum番目のウィンドウを返す
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int _windownum);
	/// <summary>
	/// ウィンドウをGameに追加する
	/// </summary>
	/// <returns>Gameで何番目のウィンドウか(ウィンドウの指定にはこの番号を使う)</returns>
	unsigned int AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height,
		LPCWSTR _windowTitle);
	//1フレーム当たりの時間
	const double mTimeEps = (double)1000 / (double)60;
	//前の処理との時間差がこの時間を超えた場合この値に矯正する
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