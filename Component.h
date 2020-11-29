#pragma once

class GameObject;
class ComponentHandle;
class Scene;
class Game;

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
class Component {
public:
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(GameObject* _owner, boost::shared_ptr<std::set<ComponentHandle*>> _hset, int _order = 0);
	/// <summary>
	/// 自身の終了処理を行う
	/// (基底クラスで持ち主のRemoveComponent(this)をする)
	/// </summary>
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	/// <param name="">経過時間</param>
	virtual void Update() = 0;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Component();
	Scene& GetScene() const;
	Game& GetGame() const;
	const int mUpdPriority;
protected:
	bool mDeleteFlag;
	GameObject& mOwner;
private:
	//自分を指すハンドルの集合のポインタ
	boost::shared_ptr<std::set<ComponentHandle*>> mHandles;
};