//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Game;
#include "Scene.h"
#include "boost/pool/pool_alloc.hpp"

/// <summary>
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
	std::weak_ptr<T> AddUpdateComponent(Args... args);
	/// <summary>
	/// OutputComponentを追加
	/// </summary>
	template<class T, class... Args>
	std::weak_ptr<T> AddOutputComponent(Args... args);
	void SetDeleteFlag();
	bool GetDeleteFlag() const;
	//すべての有効な子componentにdelete flagを付ける
	void SetAllCompsFlag();
	//子Componentの内消去済みのものを登録解除する(Scene::ProcessPandingComps用)
	void UnregisterInvalidChilds();
	//this_sh_をセットする(Scene::AddObjectから一度だけ呼び出される)
	void SetSharedPtr(std::shared_ptr<GameObject> obj);
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
	std::list<std::weak_ptr<Component>,
		boost::fast_pool_allocator<std::weak_ptr<Component>,CompsAllocator>> comps_;
	boost::mutex comps_mutex_;
	bool delete_flag_;
	//自分を指すweak_ptr
	std::weak_ptr<GameObject> this_wk_;
};

template<class T, class ...Args>
inline std::weak_ptr<T> GameObject::AddUpdateComponent(Args ...args)
{
	std::weak_ptr<T> handle = scene_->AddUpdateComponent<T>(this_wk_, args...);
	if (!handle.expired()) {
		{
			boost::unique_lock<boost::mutex> lock(comps_mutex_);
			comps_.push_back(handle);
		}
		if (delete_flag_) {
			handle.lock()->SetDeleteFlag();
		}
	}
	return handle;
}

template<class T, class ...Args>
inline std::weak_ptr<T> GameObject::AddOutputComponent(Args ...args)
{
	std::weak_ptr<T> handle = scene_->AddOutputComponent<T>(this_wk_, args...);
	if (!handle.expired()) {
		{
			boost::unique_lock<boost::mutex> lock(comps_mutex_);
			comps_.push_back(handle);
		}
		if (delete_flag_) {
			handle.lock()->SetDeleteFlag();
		}
	}
	return handle;
}
