#pragma once

class Layer;
class Component;
class Game;
class Scene;

/// <summary>
/// レイヤーに含まれるオブジェクトを表すクラス
/// </summary>
class GameObject {
public:
	GameObject(boost::weak_ptr<Layer> _layer, Vector2 _pos, double _scale, double _angle, int _prio);
	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();
	void CommonUpdate();
	virtual void UniqueUpdate();
	/// <summary>
	/// 出力関数
	/// </summary>
	void Output();
	void CommonOutput();
	virtual void UniqueOutput();
	/// <summary>
	/// 更新コンポーネントを追加する
	/// </summary>
	void AddComponent(boost::shared_ptr<Component> _comp);
	/// <summary>
	/// 出力コンポーネントを追加する
	/// </summary>
	void AddOutputComponent(boost::shared_ptr<Component> _comp);
	Vector2 GetPosition() const;
	Vector2 SetPosition(Vector2 _pos);
	double GetScale() const;
	double SetScale(double _sc);
	double GetRotation() const;
	double SetRotation(double _ro);
	/// <summary>
	/// 自分の所属するレイヤー
	/// </summary>
	const boost::weak_ptr<Layer> mLayer;
	int GetUpdPriority() const;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~GameObject();
	boost::weak_ptr<Scene> GetScene() const;
	boost::weak_ptr<Game> GetGame() const;
	boost::weak_ptr<GameObject> GetWeakThis() const;
	void SetWeakThis(boost::shared_ptr<GameObject> _this);
protected:
	/// <summary>
	/// オブジェクトの中心座標
	/// </summary>
	Vector2 mPosition;
	double mScale;
	/// <summary>
	/// 回転角度(時計回り、度数)
	/// </summary>
	double mRotation;
	/// <summary>
	/// 更新コンポーネント
	/// </summary>
	std::vector<boost::shared_ptr<Component>> mUpdateComponents;
	/// <summary>
	/// 出力コンポーネント
	/// </summary>
	std::vector<boost::shared_ptr<Component>> mOutputComponents;
	void UpdateUpdComponents();
	void UpdateOutputComponents();
	void DeleteUpdComponents();
	void DeleteOutputComponents();
	//同レイヤーにおいて優先度が大きいほど先に更新される
	int mUpdPriority;
	bool mDeleteFlag;
	boost::weak_ptr<GameObject> mThis;
};