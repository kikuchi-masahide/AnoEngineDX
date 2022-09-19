//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class GameObject;

//HACK:ほんとうに必要か?オブジェクト作成を行ったらstd::weak_ptrを返すのではだめなのか?
//シリアライズ時に個体番号を記録する場合を考えてこのクラスを作っていたが、それは必要になったらGameObjectに載せるべき機能では?
class GameObjectHandle
{
public:
	GameObjectHandle();
	GameObjectHandle(std::shared_ptr<GameObject> obj);
	std::shared_ptr<GameObject> operator->() const noexcept;
	operator bool() const noexcept;
	bool operator!() const noexcept;
	void Reset() noexcept;
private:
	//HACK:こうしないと無理なのか?
	friend bool operator<(const GameObjectHandle& a, const GameObjectHandle& b);
	std::weak_ptr<GameObject> obj_;
};

bool operator<(const GameObjectHandle& a, const GameObjectHandle& b);
bool operator>(const GameObjectHandle& a, const GameObjectHandle& b);
bool operator!=(const GameObjectHandle& a, const GameObjectHandle& b);
bool operator==(const GameObjectHandle& a, const GameObjectHandle& b);

