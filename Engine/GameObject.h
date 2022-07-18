//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Scene;
class Game;
#include "ComponentHandle.h"

/// <summary>
/// シーンに含まれるオブジェクト。ほぼ子componentをまとめる機能のみ
/// </summary>
class GameObject final {
public:
	/// <param name="comphandle_reserve_num">子componentのhandleを保持するvectorの初期reserve数</param>
	GameObject(int id, int comphandle_reserve_num);
	//SceneのAddUpdate/OutputComponentから呼ばれる
	void AddComponent(ComponentHandle<Component> comp);
	//子Componentの内消去済みのものを登録解除する(Scene::ProcessPandingComps用)
	void UnregisterInvalidChilds();
	//すべての子componentにdelete flagを付ける
	void SetAllCompsFlag();
	~GameObject();
private:
	const int kObjId;
	//HACK:プール使える?
	std::vector<ComponentHandle<Component>> comps_;
};

/// <summary>
/// GameObjectを指すハンドル。特定オブジェクトに対する操作を行う場合に用いる
/// </summary>
using GameObjectHandle = unsigned int;
