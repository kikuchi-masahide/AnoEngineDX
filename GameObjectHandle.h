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
	GameObjectHandle(GameObject* _obj, boost::shared_ptr<std::set<GameObjectHandle*>> _set);
	GameObjectHandle();
	~GameObjectHandle();
	//このハンドラが指すGameObjectのみがResetを呼び出せる(_objがmObjectと一致しなければassert)
	void Reset(GameObject* _obj);
	GameObject* operator->() const noexcept;
	bool IsValid() const;
private:
	//このハンドルが指すオブジェクト
	GameObject* mObject;
	//mObjectを指すハンドルのstd::setのポインタ
	boost::shared_ptr<std::set<GameObjectHandle*>> mHandleSet;
};