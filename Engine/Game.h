//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12/DX12.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "InputSystem.h"

class Scene;
class Window;

//HACK:初期化処理などもGameで完結させる方が分かりやすいなど，Gameを派生したクラスに実装を乗せる形にした方がベターか?
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
		if (is_executing_destructor_) {
			return;
		}
		if (!is_scene_changable_) {
			if (panding_scene_ != nullptr)DeleteScene(panding_scene_);
			panding_scene_ = DBG_NEW S(this, args...);
		}
		else {
			if (current_scene_ != nullptr)DeleteScene(current_scene_);
			current_scene_ = DBG_NEW S(this, args...);
		}
	};
	/// <summary>
	/// ウィンドウをGameに追加する
	/// </summary>
	/// <param name="_windowid">非負整数値ウィンドウID(ウィンドウ，スワップチェーンの指定にはこのIDを使う)</param>
	void AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height, LPCWSTR windowTitle, int windowid);
	/// <summary>
	/// windowidに対応するウィンドウの参照を返す
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int windowid) const;
	/// <summary>
	/// ゲームループを開始する main.cppで呼び出す
	/// </summary>
	void RunLoop();
	/// <summary>
	/// 現在開いているスワップチェーンを閉じ，このウィンドウに付随するスワップチェーンを開く
	/// </summary>
	void OpenSwapChain(int windowid);
	/// <summary>
	/// 現在開いているスワップチェーンを閉じる
	/// </summary>
	void CloseSwapChain();
	/// <summary>
	/// 次のOutput終了後RunLoopを脱出し，プログラムを終了させる
	/// </summary>
	void Terminate();
	DX12 dx12_;
	//HACK:DX12に含めるか否か考える
	TextureManager tex_manager_;
	ShaderManager shader_manager_;
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
	//このポインタをdeleteしデストラクタを呼ぶ
	void DeleteScene(Scene* scene);
	Scene* current_scene_;
	Scene* panding_scene_;
	bool is_scene_changable_;
	std::map<unsigned int, boost::shared_ptr<Window>> windows_;
	std::map<unsigned int, boost::shared_ptr<DX12SwapChain>> swapchains_;
	int current_swapchain_id_;
	InputSystem input_system_;
	bool is_executing_destructor_;
	bool terminate_flag_;
};