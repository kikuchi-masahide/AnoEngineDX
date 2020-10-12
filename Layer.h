#pragma once

class Scene;
class GameObject;
class Game;

class Layer {
public:
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
	/// コンストラクタ
	/// </summary>
	/// <param name="">所属シーン</param>
	/// <param name="">クライアント座標での占有範囲</param>
	/// <param name="">優先度(大きければ大きいほど先に更新出力が行われる)</param>
	Layer(boost::weak_ptr<Scene> _scene, Rect2 _rect, int _order = 0);
	/// <summary>
	/// オブジェクトを追加する
	/// </summary>
	void AddObject(boost::shared_ptr<GameObject> _obj);
	int GetUpdPriority() const;
	const boost::weak_ptr<Scene> mScene;
	/// <summary>
	/// クライアント座標での占有範囲の取得
	/// </summary>
	Rect2 GetClientPosition() const;
	/// <summary>
	/// クライアント座標での占有範囲の変更
	/// </summary>
	void SetClientPosition(Rect2 _rect);
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Layer();
	void SetWeakThis(boost::shared_ptr<Layer> _this);
	boost::weak_ptr<Layer> GetWeakThis() const;
	boost::weak_ptr<Game> GetGame() const;
protected:
	/// <summary>
	/// 全オブジェクトのUpdate()を呼び出す
	/// </summary>
	void UpdateObjects();
	/// <summary>
	/// 全オブジェクトのOutput()を呼び出す
	/// </summary>
	void OutputObjects();
	std::vector<boost::shared_ptr<GameObject>> mObjects;
	//保留オブジェクトのvector
	std::vector <boost::shared_ptr<GameObject>> mPandingObjects;
	/// <summary>
	/// レイヤーの左上、右下座標
	/// </summary>
	Rect2 mRect;
	//優先度(大きい順に更新出力)
	int mUpdPriority;
	bool mIsObjAddable;
	void InsertObject(boost::shared_ptr<GameObject> _obj);
	bool mDeleteFlag;
	void DeleteObjects();
	boost::weak_ptr<Layer> mThis;
};