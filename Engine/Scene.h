//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObject.h"
#include "Layer.h"
#include "InputSystem.h"
#include "Math/Math.h"

class Game;
class UIScreen;
class InputSystem;

//TODO:コンポーネント、オブジェクトの初期化の方法を考える
//コンストラクタは処理の軽い処理 -> Init()を後から呼び出し本格的な初期化を行う
//Init()を呼び出していないインスタンスをハンドルから呼び出せないようにする

/// <summary>
/// シーンを表すクラス
/// </summary>
class Scene {
public:
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
	GameObjectHandle AddObject(MatVec::Vector2 pos, double scale, double angle);
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	//このシーンに更新・出力コンポーネントを追加する
	//GameObject::AddUpdate・OutputComponentから呼び出される
	void AddUpdateComponent(GameObject* obj, ComponentHandle<Component> handle);
	//このシーンに更新・出力コンポーネントを追加する
	//GameObject::AddUpdate・OutputComponentから呼び出される
	void AddOutputComponent(GameObject* obj, ComponentHandle<Component> handle);
	template<class T, class... Args>
	LayerHandle<T> AddLayer(Args... _args)
	{
		if (is_executing_destructor_) {
			return LayerHandle<T>();
		}
		T* layerp = new T(_args...);
		//直接追加してよいならばそうする
		if (is_objcomp_addable_) {
			layers_.insert(layerp);
		}
		else {
			panding_layers_.insert(layerp);
		}
		return layerp->This<T>();
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
protected:
	virtual ~Scene();
private:
	//コンポーネントを持つsetのための順序比較ファンクタ
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle<Component>& left, const ComponentHandle<Component>& right) const {
			if (!left.IsValid())return false;
			if (!right.IsValid())return true;
			return left->mUpdPriority > right->mUpdPriority;
		}
	};
	class LayerCompare {
	public:
		bool operator()(const Layer* left, const Layer* right) const {
			return left->GetZ() < right->GetZ();
		}
	};
	//自分の持つ全更新・出力コンポーネントのUpdateを呼び出す(保留コンポーネントのそれは実行しない)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	//Deleteフラグが立っているコンポーネント・オブジェクトを削除
	void DeleteObjComp();
	//Z座標昇順で取り出す(右手系!)
	std::multiset<Layer*, LayerCompare> layers_;
	std::multiset<Layer*, LayerCompare> panding_layers_;
	//自分の持つLayerのOutputを行う
	void OutputLayer();
	//DeleteFlag立ってるLayerの処理
	void DeleteLayers();
	void DeleteUIScreen();
	//UIScreenのUpdateを奥から呼び出す
	void LaunchUIScreenUpdate();
	//UIScreenのOutputを奥から呼び出す
	void LaunchOutputUIScreens();
	//このオブジェクトのポインタをdeleteしデストラクタを呼ぶ
	void DeleteObject(GameObject* _object);
	//このレイヤーのポインタをdeleteしデストラクタを呼ぶ
	void DeleteLayer(Layer* _layer);
	//保留中のオブジェクト等をマージ
	void ProcessPandings();
	GameObject* operator&() const noexcept;
	bool delete_flag_;
	friend Game;
	bool delete_check_;
	//自身の持つGameObjectのリスト及び保留中のオブジェクト
	std::list<GameObject*> objs_;
	std::vector<GameObject*> panding_objs_;
	//自身の持つ更新・出力コンポーネントのリスト，および保留コンポーネント
	std::multiset<ComponentHandle<Component>, ComponentHandleCompare> update_components_;
	std::vector<ComponentHandle<Component>> panding_update_components_;
	std::multiset<ComponentHandle<Component>, ComponentHandleCompare> output_components_;
	std::vector<ComponentHandle<Component>> panding_output_components_;
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
};