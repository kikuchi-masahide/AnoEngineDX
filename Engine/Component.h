//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
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
	Component(GameObjectHandle handle, int order = 0);
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	virtual void Update() = 0;
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	const int upd_priority_;
	const GameObjectHandle obj_;
protected:
	virtual ~Component();
	//自身を指すハンドルを返す関数
	template<class T>
	ComponentHandle<T> This()
	{
		return ComponentHandle<T>((T*)this, &handles_);
	}
private:
	friend class GameObject;
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
	//自分を指すハンドルの集合のポインタ(void*を使うのは何というかやめたい)
	std::unordered_set<void*> handles_;
	bool delete_check_;
	bool delete_flag_;
};