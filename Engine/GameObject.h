//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Game;
#include "Scene.h"
#include "ComponentHandle.h"
#include "boost/pool/pool_alloc.hpp"

/// /// <summary>
/// シーンに含まれるオブジェクト。ほぼ子componentをまとめる機能のみ
/// </summary>
class GameObject final {
public:
	Scene* const scene_;
	GameObject(Scene* scene);
	~GameObject();
	/// <summary>
	/// UpdateComponentを追加
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(Args... args);
	/// <summary>
	/// OutputComponentを追加
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(Args... args);
	void SetDeleteFlag();
	bool GetDeleteFlag() const;
	//すべての有効な子componentにdelete flagを付ける
	void SetAllCompsFlag();
	//子Componentの内消去済みのものを登録解除する(Scene::ProcessPandingComps用)
	void UnregisterInvalidChilds();
	//this_sh_をセットする(Scene::AddObjectから一度だけ呼び出される)
	void SetSharedPtr(std::shared_ptr<GameObject> obj);
	//this_sh_をリセットし、この関数の呼び出し直後にデストラクタ実行(引数は呼び出し元チェック用)
	void ResetSharedPtr(std::shared_ptr<GameObject> obj);
private:
	//comps_のアロケータとして用いる
	struct CompsAllocator {
	public:
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		static char* malloc(const size_type size) {
			return DBG_NEW char[size];
		}
		static void free(char* const p) {
			delete p;
		}
	};
	//HACK:子をGameObjectが把握する形にするならばこれが最速
	//これを無くせれば、本当にGameObjectをインスタンスとして持つ必要はなくなりメモリも浮くが、とりあえずそれは後々考える
	//あとこれをやるなら、main終わってもメモリ解放されないのはちょっとキモいので自分でプール作って載せたい
	std::list<ComponentHandle<Component>,
		boost::fast_pool_allocator<ComponentHandle<Component>,CompsAllocator>> comps_;
	boost::mutex comps_mutex_;
	bool delete_flag_;
	//自分を指すshared_ptr
	std::shared_ptr<GameObject> this_sh_;
};

template<class T, class ...Args>
inline ComponentHandle<T> GameObject::AddUpdateComponent(Args ...args)
{
	auto handle = scene_->AddUpdateComponent<T>(this_sh_, args...);
	if (handle) {
		{
			boost::unique_lock<boost::mutex> lock(comps_mutex_);
			comps_.push_back(handle);
		}
		if (delete_flag_) {
			handle->SetDeleteFlag();
		}
	}
	return handle;
}

template<class T, class ...Args>
inline ComponentHandle<T> GameObject::AddOutputComponent(Args ...args)
{
	auto handle = scene_->AddOutputComponent<T>(this_sh_, args...);
	if (handle) {
		{
			boost::unique_lock<boost::mutex> lock(comps_mutex_);
			comps_.push_back(handle);
		}
		if (delete_flag_) {
			handle->SetDeleteFlag();
		}
	}
	return handle;
}
