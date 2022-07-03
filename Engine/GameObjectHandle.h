#pragma once

class GameObject;

/// <summary>
/// GameObjectを扱う時に使うハンドラ Scene::AddChildの返り値
/// ポインタのように->演算子でメンバにアクセス可能
/// </summary>
class GameObjectHandle {
public:
	GameObjectHandle(const GameObjectHandle& _handle);
	/// <summary>
	/// SceneのAddChild内でのみ呼び出されるコンストラクタ
	/// </summary>
	GameObjectHandle(GameObject* _obj, std::unordered_set<GameObjectHandle*>* _set);
	GameObjectHandle();
	~GameObjectHandle();
	//このオブジェクトハンドルをクリアし，nullハンドルにする
	void Reset();
	GameObject* operator->() const noexcept;
	bool IsValid() const;
	GameObjectHandle& operator=(const GameObjectHandle& handle);
private:
	//このハンドルが指すオブジェクト
	GameObject* mObject;
	//mObjectを指すハンドルのstd::setのポインタ
	std::unordered_set<GameObjectHandle*>* mHandleSet;
};