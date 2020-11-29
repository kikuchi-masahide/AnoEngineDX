#pragma once

class Game;
class Layer;
class GameObject;
class GameObjectHandle;
class ComponentHandle;
//コンポーネントをシーンで優先度順に保持するためのファンクタ
class ComponentHandleCompare;

/// <summary>
/// シーンを表すクラス
/// </summary>
class Scene {
public:
	//コンストラクタを呼び出せるのは_game自身のAddChildのみ
	Scene(Game* _game);
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
	/// レイヤーを加える
	/// </summary>
	void AddLayer(boost::shared_ptr<Layer> _layer);
	GameObjectHandle AddObject(Vector2 _pos, double _scale, double _angle);
	Game &mGame;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	//現在のシーンのポインタを持てるのはGameのみであるから，
	//デストラクタを呼び出せるのもGameのみ
	virtual ~Scene();
	//このシーンに更新・出力コンポーネントを追加する
	//GameObject::AddUpdate・OutputComponentから呼び出される
	void AddOutputComponent(GameObject* _obj, ComponentHandle& _handle);
	void AddUpdateComponent(GameObject* _obj, ComponentHandle& _handle);
protected:
	/// <summary>
	/// 全レイヤーのUpdate()を呼び出す
	/// </summary>
	void UpdateLayers();
	/// <summary>
	/// 全レイヤーのOutput()を呼び出す
	/// </summary>
	void OutputLayers();
	std::vector<boost::shared_ptr<Layer>> mLayers;
	std::vector<boost::shared_ptr<Layer>> mPendingLayers;
	void InsertLayer(boost::shared_ptr<Layer> _layer);
	void DeleteLayers();
	bool mDeleteFlag;
private:
	//自身の持つGameObjectのリスト及び保留中のオブジェクト
	std::list<boost::shared_ptr<GameObject>> mObjs;
	std::vector<boost::shared_ptr<GameObject>> mPandingObjs;
	//自身の持つ更新・出力コンポーネントのリスト，および保留コンポーネント
	std::set<ComponentHandle, ComponentHandleCompare> mUpdateComponents;
	std::vector<ComponentHandle> mPandingUpdateComponents;
	std::set<ComponentHandle, ComponentHandleCompare> mOutputComponents;
	std::vector<ComponentHandle> mPandingOutputComponents;
	//自分の持つ全更新・出力コンポーネントのUpdateを呼び出す(保留コンポーネントのそれは実行しない)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	//コンポーネント・オブジェクトを直接リストに入れられるか?
	bool mIsObjCompAddable;
	//Deleteフラグが立っているコンポーネント・オブジェクトや保留中のそれらの処理
	void DeleteAndProcessPandingObjComp();
};