#pragma once

class Scene;
class Game;

#include "ComponentHandle.h"
#include "GameObjectHandle.h"

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
// HACK:コンポーネントをこのクラスの派生にせず，処理の中身・持つデータを型クラスにするのはありか?
class Component {
public:
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(GameObjectHandle _handle, int _order = 0);
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	virtual void Update() = 0;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	const int mUpdPriority;
	const GameObjectHandle mObj;
protected:
	virtual ~Component();
	bool mDeleteFlag;
	//自身を指すハンドルを返す関数
	template<class T>
	ComponentHandle<T> This()
	{
		return ComponentHandle<T>((T*)this, &mHandles);
	}
private:
	bool mDeleteCheck;
	friend class GameObject;
	//自分を指すハンドルの集合のポインタ(void*を使うのは何というかやめたい)
	std::unordered_set<void*> mHandles;
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
};