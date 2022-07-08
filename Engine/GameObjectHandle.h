//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class GameObject;

/// <summary>
/// GameObjectを扱う時に使うハンドラ Scene::AddChildの返り値
/// ポインタのように->演算子でメンバにアクセス可能
/// </summary>
class GameObjectHandle {
public:
	GameObjectHandle(const GameObjectHandle& handle);
	/// <summary>
	/// SceneのAddChild内でのみ呼び出されるコンストラクタ
	/// </summary>
	GameObjectHandle(GameObject* obj, std::unordered_set<GameObjectHandle*>* set);
	GameObjectHandle();
	GameObjectHandle& operator=(const GameObjectHandle& handle);
	~GameObjectHandle();
	bool IsValid() const;
	GameObject* operator->() const noexcept;
	//このオブジェクトハンドルをクリアし，nullハンドルにする
	void Reset();
private:
	//このハンドルが指すオブジェクト
	GameObject* object_;
	//mObjectを指すハンドルのstd::setのポインタ
	std::unordered_set<GameObjectHandle*>* handles_set_;
};