//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Scene;
class Game;
#include "ComponentHandle.h"
#include "boost/pool/pool_alloc.hpp"

/// /// <summary>
/// シーンに含まれるオブジェクト。ほぼ子componentをまとめる機能のみ
/// </summary>
class GameObject final {
public:
	/// <param name="comphandle_reserve_num">子componentのhandleを保持するvectorの初期reserve数</param>
	GameObject(int id);
	//SceneのAddUpdate/OutputComponentから呼ばれる
	void AddComponent(ComponentHandle<Component> comp);
	//子Componentの内消去済みのものを登録解除する(Scene::ProcessPandingComps用)
	void UnregisterInvalidChilds();
	//すべての有効な子componentにdelete flagを付ける
	void SetAllCompsFlag();
	~GameObject();
private:
	const int kObjId;
	struct myallocator {
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
	std::list<ComponentHandle<Component>, boost::fast_pool_allocator<ComponentHandle<Component>, myallocator>> comps_;
};

/// <summary>
/// GameObjectを指すハンドル。特定オブジェクトに対する操作を行う場合に用いる
/// </summary>
using GameObjectHandle = unsigned int;
