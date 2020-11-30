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
	/// コンポ－ネントの更新処理
	/// </summary>
	virtual void Update() = 0;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Component();
	Scene& GetScene() const;
	Game& GetGame() const;
	GameObject& mOwner;
	const int mUpdPriority;
protected:
	bool mDeleteFlag;
private:
	//自分を指すハンドルの集合のポインタ
	boost::shared_ptr<std::set<ComponentHandle*>> mHandles;
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
};