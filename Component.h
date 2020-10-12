#pragma once

class GameObject;
class Layer;
class Scene;
class Game;

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
class Component {
public:
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(boost::weak_ptr<GameObject>& _owner, int _order = 0);
	/// <summary>
	/// 自身の終了処理を行う
	/// (基底クラスで持ち主のRemoveComponent(this)をする)
	/// </summary>
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	/// <param name="">経過時間</param>
	virtual void Update();
	int GetUpdPriority() const { return mUpdPriority; };
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Component();
	boost::weak_ptr<Layer> GetLayer() const;
	boost::weak_ptr<Scene> GetScene() const;
	boost::weak_ptr<Game> GetGame() const;
	void SetWeakThis(boost::shared_ptr<Component>& _this);
	boost::weak_ptr<Component> GetWeakThis() const;
	const boost::weak_ptr<GameObject> mOwner;
protected:
	boost::weak_ptr<Component> mThis;
	int mUpdPriority;
	bool mDeleteFlag;
};