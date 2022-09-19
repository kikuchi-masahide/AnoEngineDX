//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12/Master.h"
#include "InputSystem.h"
#include "FontMaster.h"
#include "AudioMaster.h"

class Scene;
class Window;

class Game {
public:
	//1フレーム当たりの時間
	static constexpr double kFrameTimeDelta = 1000.0 / 60;
	//前の処理との時間差がこの時間を超えた場合、この値に矯正する
	static constexpr double kProcessTimeDeltaLimit = kFrameTimeDelta * 3;
	Game();
	virtual ~Game();
	/// <summary>
	/// 次フレームからどのシーンに移行するか指定する．
	/// (実際にシーンが変わるのは次フレームからだが，次シーンの初期化が行われるのは呼び出し時なのが困る)
	/// </summary>
	template<class S, class... Args>
	void ChangeScene(Args... args) {
		//HACK:ここの遅延初期化、およびコンストラクタでpandingに入れたのをUpdate前に本配列に移す処理
		if (is_executing_destructor_) {
			return;
		}
		//現在実行中の場合中断を通告
		async_initing_thread_.interrupt();
		async_initing_thread_.join();
		Scene* newscene = DBG_NEW S(this, args...);
		boost::thread th(&Game::AsyncInitializeScene, this, newscene);
		async_initing_thread_.swap(th);
	};
	/// <summary>
	/// ウィンドウをGameに追加する
	/// </summary>
	/// <param name="_windowid">整数値ウィンドウID(ウィンドウ，スワップチェーンの指定にはこのIDを使う)</param>
	/// <returns>作成したwindow</returns>
	std::weak_ptr<Window> AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height,
		LPCWSTR windowTitle, int windowid);
	/// <summary>
	/// windowidに対応するウィンドウのポインタを返す
	/// </summary>
	std::shared_ptr<Window> GetWindow(int windowid) const;
	/// <summary>
	/// ゲームループを開始する main.cppで呼び出す
	/// </summary>
	void RunLoop();
	/// <summary>
	/// 次のOutput終了後RunLoopを脱出し，プログラムを終了させる
	/// </summary>
	void Terminate();
	DX12::Master dx12_;
	FontMaster fontmaster_;
	AudioMaster audiomaster_;
protected:
	/// <summary>
	/// UpdateGame内で、Sceneの1回のUpdateを行う前の処理を行う
	/// </summary>
	virtual void BeforeUpdate();
	/// <summary>
	/// UpdateGame内で、Sceneの1回のUpdateを行った後の処理を行う
	/// </summary>
	virtual void AfterUpdate();
	/// <summary>
	/// GenerateOutput内で、SceneのOutputを行う前の処理を行う
	/// </summary>
	virtual void BeforeOutput();
	/// <summary>
	/// GenerateOutput内で、SceneのOutputを行った後の処理を行う
	/// </summary>
	virtual void AfterOutput();
private:
	/// <summary>
	/// 入力処理(2フレーム分以上の更新処理を行う場合、キー入力は1フレーム目の更新直前のみ行うため、BeforeUpdateには含めない)
	/// </summary>
	void ProcessInput();
	/// <summary>
	/// ゲーム全体の更新処理
	/// </summary>
	void UpdateGame();
	/// <summary>
	/// ゲーム全体の出力生成
	/// </summary>
	void GenerateOutput();
	/// <summary>
	/// ChangeSceneにて別スレッドで実行する、sceneのAsyncInitializeを実行する関数
	/// </summary>
	/// <param name="scene"></param>
	void AsyncInitializeScene(Scene* scene);
	//フレームの初めで、scene_を更新
	void ProcessPandingScene();
	//このポインタをdeleteしデストラクタを呼ぶ
	void DeleteScene(Scene* scene);
	Scene* current_scene_;
	Scene* panding_scene_;
	boost::mutex panding_scene_mutex_;
	std::map<unsigned int, std::shared_ptr<Window>> windows_;
	int current_swapchain_id_;
	InputSystem input_system_;
	bool is_executing_destructor_;
	bool terminate_flag_;
	//AsyncInitializeSceneを動かしているスレッド
	boost::thread async_initing_thread_;
};