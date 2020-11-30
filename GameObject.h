#pragma once

#include "Scene.h"

class Component;
class Game;
class GameObjectHandle;
class ComponentHandle;

template<class T>
concept ComponentInheritenc = requires (T & x) {

}

/// <summary>
/// シーンに含まれるオブジェクトを表すクラス
/// </summary>
class GameObject final {
public:
	/// <summary>
	/// Scene::AddChildから呼び出されるコンストラクタ
	/// </summary>
	GameObject(Scene* _scene, boost::shared_ptr<std::set<GameObjectHandle*>> _hset, Vector2 _pos, double _scale, double _angle);
	Vector2 GetPosition() const;
	Vector2 SetPosition(Vector2 _pos);
	double GetScale() const;
	double SetScale(double _sc);
	double GetRotation() const;
	double SetRotation(double _ro);
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	~GameObject();
	//テンプレートの混じる関数の定義を.hを置かないといけないのは何故か一応調べる
	template<class T,class... Args>
	ComponentHandle AddUpdateComponent(Args... _args) {
		//このコンポーネントを指すハンドルの集合
		boost::shared_ptr<std::set<ComponentHandle*>> comphsetp(new std::set<ComponentHandle*>());
		//コンポーネント自身
		boost::shared_ptr<Component> compp(new T(this, comphsetp, _args...));
		mUpdateComponents.push_back(compp);
		//返すハンドル
		ComponentHandle comph(compp.get(), comphsetp);
		//シーンに追加
		mScene->AddUpdateComponent(this,comph);
		return comph;
	};
	template<class T, class... Args>
	ComponentHandle AddOutputComponent(Args... _args) {
		//このコンポーネントを指すハンドルの集合
		boost::shared_ptr<std::set<ComponentHandle*>> comphsetp(new std::set<ComponentHandle*>());
		//コンポーネント自身
		boost::shared_ptr<Component> compp(new T(this, comphsetp, _args...));
		mOutputComponents.push_back(compp);
		//返すハンドル
		ComponentHandle comph(compp.get(), comphsetp);
		//シーンに追加
		mScene->AddOutputComponent(this,comph);
		return comph;
	};
	//フラグが立っているコンポーネントを削除
	void DeleteFlagedComponents(Scene* _scene);
	Scene& GetScene() const { return *mScene; };
	Game& GetGame() const;
private:
	Scene* mScene;
	/// <summary>
	/// オブジェクトの中心座標
	/// </summary>
	Vector2 mPosition;
	double mScale;
	/// <summary>
	/// 回転角度(時計回り、度数)
	/// </summary>
	double mRotation;
	//このオブジェクトの持つ更新・出力コンポーネント
	std::list<boost::shared_ptr<Component>> mUpdateComponents;
	std::list<boost::shared_ptr<Component>> mOutputComponents;
	//このオブジェクトを指すハンドルのset
	boost::shared_ptr<std::set<GameObjectHandle*>> mHandles;
	bool mDeleteFlag;
	GameObject* operator&() const noexcept;
};