//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObject.h"
#include "ComponentHandle.h"

class Scene;

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
class Component {
public:
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(Scene* scene, GameObjectHandle handle, int order = 0);
	virtual void Initialize();
	/// <summary>
	/// Sceneのデストラクタが実行された際の呼び出しなどでは、Add~Componentしても必ず有効なComponentが返ってくるとは限らないことに注意
	/// </summary>
	virtual ~Component();
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	virtual void Update() = 0;
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	//自分を指すshared_ptrを設定する(Thisの際に用いる)
	void SetSelfSharedptr(std::shared_ptr<Component> comp);
	const int upd_priority_;
	GameObjectHandle const obj_;
	//HACK:GameObjectにscene_を移せば、メモリ削減にはなる。する意味はあるか?現状sizeof(Component)=48
	Scene* const scene_;
	//自身を指すハンドルを返す関数
	template<class T>
	ComponentHandle<T> This()
	{
#ifdef _DEBUG
		//デバッグ時はdynamic_pointer_castを行い安全な型変換か確認する
		if (std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(self_.lock())) {
			return ComponentHandle<T>(result);
		}
		else {
			Log::OutputCritical("Component::This() for bad type of ComponentHandle");
			return ComponentHandle<T>();
		}
#else
		return ComponentHandle<T>(std::static_pointer_cast<T>(self_.lock()));
#endif
	}
protected:
private:
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
	std::weak_ptr<Component> self_;
	bool delete_flag_;
};