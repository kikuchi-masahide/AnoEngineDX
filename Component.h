#pragma once

class GameObject;
class Scene;
class Game;
template<class T>
class ComponentHandle;

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
class Component {
public:
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, int _order = 0);
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
	template<class T>
	ComponentHandle<T> This()
	{
		return ComponentHandle<T>((T*)this, mHandles);
	}
	bool mDeleteFlag;
private:
	//自分を指すハンドルの集合のポインタ(void*を使うのは何というかやめたい)
	boost::shared_ptr<std::set<void*>> mHandles;
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
};