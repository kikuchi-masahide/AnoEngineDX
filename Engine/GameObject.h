#pragma once

#include "Component.h"
#include "GameObjectHandle.h"
#include "Math/Math.h"

class Scene;
class Game;
template<class T>
class ComponentHandle;

/// <summary>
/// シーンに含まれるオブジェクトを表すクラス
/// </summary>
class GameObject final {
public:
	Scene* const scene_;
	/// <summary>
	/// Scene::AddChildから呼び出されるコンストラクタ
	/// </summary>
	GameObject(Scene* const scene, MatVec::Vector2 pos, double scale, double angle);
	/// <summary>
	/// このオブジェクトにT型のUpdateComponentを追加(_argsはコンストラクタに渡す引数で，第一引数mObjを除いたもの)
	/// concept等でTの正当性を後々(静的に)評価したい
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(Args... args) {
		//コンポーネント自身
		T* comp = new T(This(), args...);
		update_components_.push_back(comp);
		ComponentHandle<T> handle = comp->This<T>();
		//シーンに追加
		AddUpdateComponentToScene(static_cast<ComponentHandle<Component>>(handle));
		return handle;
	}
	/// <summary>
	/// このオブジェクトにT型のOutputComponentを追加(_argsはコンストラクタに渡す引数で，第一引数mObjを除いたもの)
	/// concept等でTの正当性を後々(静的に)評価したい
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(Args... args) {
		//コンポーネント自身
		T* comp = new T(This(), args...);
		output_components_.push_back(comp);
		ComponentHandle<T> handle = comp->This<T>();
		//シーンに追加
		AddOutputComponentToScene(static_cast<ComponentHandle<Component>>(handle));
		return handle;
	};
	MatVec::Vector2 GetPosition() const;
	MatVec::Vector2 SetPosition(MatVec::Vector2 pos);
	double GetScale() const;
	double SetScale(double sc);
	double GetRotation() const;
	double SetRotation(double ro);
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	Game& GetGame();
private:
	friend class Scene;
	~GameObject();
	//AddUpdateComponent内で呼ばれる
	//#includeの方向を一致させるため別関数に分離
	void AddUpdateComponentToScene(ComponentHandle<Component> handle);
	void AddOutputComponentToScene(ComponentHandle<Component> handle);
	/// <summary>
	/// Scene::AddObjectで呼び出される，自身を指すハンドルを返す関数
	/// </summary>
	GameObjectHandle This();
	//このコンポーネントをdeleteしデストラクタを呼ぶ
	//(もともとこのポインタが入っていたlistからのeraseはしない)
	void DeleteComponent(Component* component);
	//フラグが立っているコンポーネントを削除
	void DeleteFlagedComponents();
	/// <summary>
	/// オブジェクトの中心座標
	/// </summary>
	MatVec::Vector2 position_;
	double scale_;
	/// <summary>
	/// 回転角度(時計回り、度数)
	/// </summary>
	double rotation_;
	//このオブジェクトの持つ更新・出力コンポーネント
	std::list<Component*> update_components_;
	std::list<Component*> output_components_;
	//このオブジェクトを指すハンドルのset
	std::unordered_set<GameObjectHandle*> handles_;
	bool delete_flag_;
	GameObject* operator&() const noexcept;
};