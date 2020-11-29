#pragma once

#include "ComponentHandle.h"
#include "Component.h"

class Game;
class GameObject;
class GameObjectHandle;

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
	virtual void UniqueUpdate();
	/// <summary>
	/// 出力関数
	/// </summary>
	void Output();
	virtual void UniqueOutput();
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
	bool mDeleteFlag;
private:
	//自身の持つGameObjectのリスト及び保留中のオブジェクト
	std::list<boost::shared_ptr<GameObject>> mObjs;
	std::vector<boost::shared_ptr<GameObject>> mPandingObjs;
	//コンポーネントを持つsetのための順序比較ファンクタ
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle& left, const ComponentHandle& right) const {
			return left->mUpdPriority < right->mUpdPriority;
		}
	};
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