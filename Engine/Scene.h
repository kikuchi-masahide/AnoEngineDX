//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObject.h"
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
	GameObjectHandle AddObject(int comphandle_reserve_num = 4);
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	/// <summary>
	/// objの指すGameObjectにUpdateComponentを追加
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(GameObjectHandle obj, Args... args) {
		//HACK:Sceneのデストラクタ実行時、panding_~_comps_が無限に増えていくことを防止
		//コンストラクタ自体の実行をAdd~Componentで行わなければこれ要らないんだけど......
		if (is_executing_destructor_) {
			return ComponentHandle<T>();
		}
		auto itr = id_objpointer_map_.find(obj);
		if (itr == id_objpointer_map_.end()) {
			Log::OutputCritical("AddUpdateComponent to unexisting GameObject");
			return ComponentHandle<T>();
		}
		T* comp_p = DBG_NEW T(this, obj, args...);
		panding_update_components_.push_back(comp_p);
		itr->second->AddComponent(comp_p->This<Component>());
		return comp_p->This<T>();
	}
	/// <summary>
	/// objの指すGameObjectにOutputComponentを追加
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(GameObjectHandle obj, Args... args) {
		if (is_executing_destructor_) {
			return ComponentHandle<T>();
		}
		auto itr = id_objpointer_map_.find(obj);
		if (itr == id_objpointer_map_.end()) {
			Log::OutputCritical("AddOutputComponent to unexisting GameObject");
			return;
		}
		T* comp_p = DBG_NEW T(this, obj, args...);
		panding_output_components_.push_back(comp_p);
		itr->second->AddComponent(comp_p->This<Component>());
		return comp_p->This<T>();
	}
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

	void Erase(GameObjectHandle handle);
	int GetGameObjectNumber();
	int GetUpdateComponentNumber();
	int GetOutputComponentNumber();

protected:
	virtual ~Scene();
private:
	//自分の持つ全更新・出力コンポーネントのUpdateを呼び出す(保留コンポーネントのそれは実行しない)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	void DeleteUIScreen();
	//UIScreenのUpdateを奥から呼び出す
	void LaunchUIScreenUpdate();
	//UIScreenのOutputを奥から呼び出す
	void LaunchOutputUIScreens();
	//このオブジェクトのポインタをdeleteしデストラクタを呼ぶ
	void DeleteObject(GameObject* _object);
	GameObject* operator&() const noexcept;
	bool delete_flag_;
	friend Game;
	bool delete_check_;
	//自身の持つGameObjectのリスト及び保留中のオブジェクト
	std::vector<GameObject*> objs_;
	//自身の持つ更新・出力コンポーネントのリスト，および保留コンポーネント
	//HACK:余裕あったら別のコンテナに変えた場合のパフォーマンス比較
	std::vector<Component*> update_components_;
	std::vector<Component*> panding_update_components_;
	std::vector<Component*> output_components_;
	std::vector<Component*> panding_output_components_;
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

	constexpr static int kMaxObjNum_ = 10000;
	//GameObjectを保存するメモリプール
	static std::optional<boost::pool<>> obj_pool_;
	//HACK:アロケータ載せてプール使う?
	static std::map<GameObjectHandle, GameObject*> id_objpointer_map_;
	//次にGameObjectを追加するとき使うid
	int next_obj_id_;
	//次消すべきオブジェクトのid
	std::vector<GameObjectHandle> erase_objs_;
	void ProcessPandingComps();
	void ProcessPandingUIScreens();
};