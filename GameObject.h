#pragma once

class Layer;
class Component;
class Game;
class Scene;
class GameObjectHandle;
class ComponentHandle;

/// <summary>
/// レイヤーに含まれるオブジェクトを表すクラス
/// </summary>
class GameObject final {
public:
	/// <summary>
	/// Scene::AddChildから呼び出されるコンストラクタ
	/// </summary>
	GameObject(Scene* _scene, boost::shared_ptr<std::set<GameObjectHandle*>> _hset, Vector2 _pos, double _scale, double _angle);
	///// <summary>
	///// 更新関数
	///// </summary>
	//void Update();
	///// <summary>
	///// 出力関数
	///// </summary>
	//void Output();
	Vector2 GetPosition() const;
	Vector2 SetPosition(Vector2 _pos);
	double GetScale() const;
	double SetScale(double _sc);
	double GetRotation() const;
	double SetRotation(double _ro);
	Scene& mScene;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	~GameObject();
	template<class T,class... Args>
	ComponentHandle AddUpdateComponent(Args... _args);
	template<class T, class... Args>
	ComponentHandle AddOutputComponent(Args... _args);
private:
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
};