//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "ComponentHandle.h"
#include "GameObjectHandle.h"

class Scene;

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
class Component {
public:
	const int upd_priority_;
	GameObjectHandle const obj_;
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(GameObjectHandle handle, int order = 0);
	/// <summary>
	/// コンストラクタの実行後、別スレッドで呼び出される初期化関数
	/// (オーバーライド先で基本クラスのAsyncInitialize()を呼び出すこと)
	/// </summary>
	virtual void AsyncInitialize();
	/// <summary>
	/// Sceneのデストラクタが実行された際の呼び出しなどでは、Add~Componentしても必ず有効なComponentが返ってくるとは限らないことに注意
	/// </summary>
	virtual ~Component();
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	virtual void Update() = 0;
	//自身を指すハンドルを返す関数
	template<class T>
	ComponentHandle<T> This()
	{
#ifdef _DEBUG
		//デバッグ時はdynamic_pointer_castを行い安全な型変換か確認する
		if (std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(this_sh_)) {
			return ComponentHandle<T>(result);
		}
		else {
			Log::OutputCritical("Component::This() for bad type of ComponentHandle");
			return ComponentHandle<T>();
		}
#else
		return ComponentHandle<T>(std::static_pointer_cast<T>(this_sh_));
#endif
	}
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	//this_sh_をセットする(Scene::AddComponentから一度だけ呼び出される)
	void SetSharedPtr(std::shared_ptr<Component> comp);
	//this_sh_をリセットし、この関数の呼び出し直後にデストラクタ実行(引数は呼び出し元チェック用)
	void ResetSharedPtr(std::shared_ptr<Component> comp);
protected:
private:
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
	std::shared_ptr<Component> this_sh_;
	bool delete_flag_;
};