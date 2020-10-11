#pragma once

class Texture;
class VArray;
class Shader;
struct GLFWwindow;
class Scene;
class FBTexture;
class FlipAnimationData;
class MouseInput;
class KeyboardInput;
class Window;

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();
	Game();
	void ChangeScene(Scene* _scene);
	/// <summary>
	/// このインスタンスで_windownum番目のウィンドウを返す
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int _windownum);
	/// <summary>
	/// ウィンドウをGameに追加する
	/// </summary>
	/// <returns>Gameで何番目のウィンドウか(ウィンドウの指定にはこの番号を使う)</returns>
	int AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height,
		LPCWSTR _windowTitle);
	//1フレーム当たりの時間
	const double mTimeEps = (double)1000 / (double)60;
	//前の処理との時間差がこの時間を超えた場合この値に矯正する
	const double mProcessTimeLimit = mTimeEps * 3;
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
	void DeleteScene(Scene* _scene);
};