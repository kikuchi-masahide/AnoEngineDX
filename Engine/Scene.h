//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObjectHandle.h"
#include "ElementContainer.h"
#include "Component.h"
#include "InputSystem.h"
#include "Math/Math.h"

class Game;
class UIScreen;
class InputSystem;

//HACK:プロファイラができたら試すこと
//・コンポーネントの初期化をstd::anyでもなんでも使って、Add~Componentで行わないようにする
//・ComponentHandleでstd::weak_ptrを使えないか?使った場合の速度比較
//・std::setなどをstd::vector等別のコンテナに変えた場合の比較
//・poolを使えるところでもっと使った場合の比較

/// <summary>
/// シーンを表すクラス
/// </summary>
class Scene {
public:
	/// <summary>
	/// プールなどの初期化。Gameのコンストラクタから呼び出す
	/// </summary>
	static void InitMemory();
	//コンストラクタを呼び出せるのは_game自身のAddChildのみ
	Scene(Game* const game);
	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const InputSystem* input);
	/// <summary>
	/// GameObjectやUIScreenや更新処理の前に実行される，override可能な独自処理
	/// </summary>
	virtual void PriorUniqueUpdate();
	/// <summary>
	/// GameObjectやUIScreenや更新処理の後に実行される，override可能な独自処理
	/// </summary>
	virtual void PosteriorUniqueUpdate();
	/// <summary>
	/// 出力関数
	/// </summary>
	void Output();
	/// <summary>
	/// GameObject等のUpdateの前に実行される，override可能な独自処理
	/// </summary>
	virtual void PriorUniqueOutput();
	/// <summary>
	/// GameObject等のUpdateの後に実行される，override可能な独自処理
	/// (このフレームでのGameObject，Component追加はまだ保留状態)
	/// </summary>
	virtual void PosteriorUniqueOutput();
	GameObjectHandle AddObject();
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	/// <summary>
	/// objの指すGameObjectにUpdateComponentを追加(GameObject::AddUpdateComponentから呼び出される)
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(std::shared_ptr<GameObject> obj, Args... args);
	/// <summary>
	/// objの指すGameObjectにOutputComponentを追加(GameObject::AddOutputComponentから呼び出される)
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(std::shared_ptr<GameObject> obj, Args... args);
	/// <summary>
	/// このupd_prioを持つコンポーネントを実行する前に実行する関数を登録する
	/// (コンポーネントが存在しなければ実行しない)
	/// </summary>
	void SetOutputCompsPreFunc(int upd_prio, std::function<void()> func);
	/// <summary>
	/// このupd_prioを持つ最後のコンポーネントを実行した後に実行する関数を登録する
	/// (コンポーネントが存在しなければ実行しない)
	/// </summary>
	void SetOutputCompsPostFunc(int upd_prio, std::function<void()> func);
	/// <summary>
	/// UIScreenを継承するクラスの追加
	/// </summary>
	/// <param name="..._args">コンストラクタに渡す引数</param>
	template<class T, class... Args>
	T* AddUIScreen(Args... _args)
	{
		if (is_executing_destructor_) {
			return nullptr;
		}
		if (is_objcomp_addable_) {
			if (uiscreens_.size() == 0) {
				prev_mouse_pos_for_uiscreens_.push_back(prev_mouse_pos_for_comps_);
			}
			else {
				prev_mouse_pos_for_uiscreens_.push_back(prev_mouse_pos_for_uiscreens_.back());
			}
			auto ptr = new T(this, _args...);
			uiscreens_.push_back(ptr);
			update_flag_for_uiscreens_.push_back(true);
			input_flag_for_uiscreens_.push_back(true);
			return ptr;
		}
		else {
			auto ptr = new T(this, _args...);
			panding_uiscreens_.push_back(ptr);
			return ptr;
		}
	}
	/// <summary>
	///各キー・マウスのボタンの状態を受け取る
	/// </summary>
	ButtonState GetKeyState(int _key) const;
	/// <summary>
	//マウスのクライアント座標を得る(左下原点)
	/// </summary>
	MatVec::Vector2 GetMouseClientPos(int windowid) const;
	/// <summary>
	//マウスの移動ベクトルを得る(左下原点)
	/// </summary>
	MatVec::Vector2 GetMouseMove() const;
	/// <summary>
	//マウスのスクリーン座標を得る(左上原点)
	/// </summary>
	MatVec::Vector2 GetMouseScreenPos() const;
	Game& game_;

	//このGameObjectを、フレーム切り替え前に消去する(GameObject::SetDeleteFlagから呼び出される)
	void Erase(std::weak_ptr<GameObject> ptr);
	void Erase(std::weak_ptr<Component> ptr);
	int GetGameObjectNumber();
	int GetUpdateComponentNumber();
	int GetOutputComponentNumber();
protected:
	virtual ~Scene();
private:
	//自分の持つ全更新・出力コンポーネントのUpdateを呼び出す(保留コンポーネントのそれは実行しない)
	void DeleteUIScreen();
	//UIScreenのUpdateを奥から呼び出す
	void LaunchUIScreenUpdate();
	//UIScreenのOutputを奥から呼び出す
	void LaunchOutputUIScreens();
	GameObject* operator&() const noexcept;
	bool delete_flag_;
	friend Game;
	bool delete_check_;
	//コンポーネント・オブジェクトを直接リストに入れられるか?
	bool is_objcomp_addable_;
	//持っているUIScreen群(添え字の大きいものが後に追加されたUIScreen)
	std::vector<UIScreen*> uiscreens_;
	//保留UIScreen
	std::vector<UIScreen*> panding_uiscreens_;
	const InputSystem* input_system_;
	//今tick，Componentに入力情報を渡すか否か
	bool input_flag_for_comps_;
	//今tick，ComponentのUpdateを実行するか否か
	bool update_flag_for_comps_;
	//今tick，対応UIScreenに入力情報を渡すか否か
	std::vector<bool> input_flag_for_uiscreens_;
	//今tick，対応UIScreenのUpdateを実行するか否か
	std::vector<bool> update_flag_for_uiscreens_;
	//GetButtonState等で使う，入力情報を渡すか否かのフラグ
	bool input_flag_;
	//GetClientMousePos等で使う，前tickでのマウス位置
	MatVec::Vector2 prev_mouse_pos_;
	//Compsにとっての前tickマウス位置(左上原点スクリーン座標)
	MatVec::Vector2 prev_mouse_pos_for_comps_;
	//UIScreenにとっての前tickマウス位置(左上原点スクリーン座標)
	std::vector<MatVec::Vector2> prev_mouse_pos_for_uiscreens_;
	//デストラクタ実行時のみtrue
	bool is_executing_destructor_;
	void ProcessPandingComps();
	void ProcessPandingUIScreens();
	ElementContainer element_container_;
};

template<class T, class ...Args>
inline ComponentHandle<T> Scene::AddUpdateComponent(std::shared_ptr<GameObject> obj, Args ...args)
{
	if (is_executing_destructor_) {
		return ComponentHandle<T>();
	}
	return element_container_.AddUpdateComponent<T>(obj, args...);
}

template<class T, class ...Args>
inline ComponentHandle<T> Scene::AddOutputComponent(std::shared_ptr<GameObject> obj, Args ...args)
{
	if (is_executing_destructor_) {
		return ComponentHandle<T>();
	}
	return element_container_.AddOutputComponent<T>(obj, args...);
}

