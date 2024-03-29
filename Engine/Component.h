//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Log.h"

class Scene;
class GameObject;

/// <summary>
/// オブジェクトの機能を表すクラス
/// オブジェクトの更新をするコンポーネントと出力をするコンポーネントは両方この基底クラスから派生させる
/// </summary>
class Component {
public:
	const int upd_priority_;
	const std::weak_ptr<GameObject> obj_;
	/// <param name="_order">優先度 高いほど先に呼び出される</param>
	Component(std::weak_ptr<GameObject> obj, int order = 0);
	/// <summary>
	/// コンストラクタの実行後、別スレッドで呼び出される初期化関数
	/// (オーバーライド先で基本クラスのAsyncInitialize()を呼び出すこと)
	/// </summary>
	virtual void AsyncInitialize();
	/// <summary>
	/// このコンポーネントの消去処理を行う(基底クラスのCleanUp呼び出しを最初に行う必要がある)
	/// Sceneのデストラクタが実行された際の呼び出しなどでは、Add~Componentしても必ず有効なComponentが返ってくるとは限らないことに注意
	/// </summary>
	virtual void CleanUp();
	~Component();
	/// <summary>
	/// コンポ－ネントの更新処理
	/// </summary>
	virtual void Update() = 0;
	//自身を指すハンドルを返す関数
	template<class T>
	std::weak_ptr<T> This()
	{
#ifdef _DEBUG
		//デバッグ時はdynamic_pointer_castを行い安全な型変換か確認する
		if (std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(this_wk_.lock())) {
			return result;
		}
		else {
			Log::OutputCritical("Component::This() for bad type of");
			return nullptr;
		}
#else
		return ComponentHandle<T>(std::static_pointer_cast<T>(this_wk_.lock()));
#endif
	}
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	//this_wk_をセットする(Scene::AddComponentから一度だけ呼び出される)
	void SetSharedPtr(std::shared_ptr<Component> comp);
protected:
private:
	//&でインスタンスのポインタを取得させない
	Component* operator&() const noexcept;
	std::weak_ptr<Component> this_wk_;
	bool delete_flag_;
};