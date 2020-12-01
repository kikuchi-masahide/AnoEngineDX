#pragma once

#include "Component.h"
#include "ComponentHandle.h"

//class Component;
//template<class T>
//class ComponentHandle;
class Game;
class GameObject;
class GameObjectHandle;

class ComponentHandleCompare {
public:
	bool operator()(const ComponentHandle<Component>& left, const ComponentHandle<Component>& right) const {
		return left->mUpdPriority < right->mUpdPriority;
	}
};

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
	template<class T>
	void AddOutputComponent(GameObject* _obj, ComponentHandle<T> _handle)
	{
		assert(_obj != nullptr);
		if (mIsObjCompAddable)mOutputComponents.insert(static_cast<ComponentHandle<Component>>(_handle));
		else mPandingOutputComponents.push_back(static_cast<ComponentHandle<Component>>(_handle));
	}
	template<class T>
	void AddUpdateComponent(GameObject* _obj, ComponentHandle<T> _handle)
	{
		assert(_obj != nullptr);
		assert(_handle.IsValid());
		if (mIsObjCompAddable)mUpdateComponents.insert(static_cast<ComponentHandle<Component>>(_handle));
		else mPandingUpdateComponents.push_back(static_cast<ComponentHandle<Component>>(_handle));
	}
protected:
	bool mDeleteFlag;
private:
	//自身の持つGameObjectのリスト及び保留中のオブジェクト
	std::list<boost::shared_ptr<GameObject>> mObjs;
	std::vector<boost::shared_ptr<GameObject>> mPandingObjs;
	//コンポーネントを持つsetのための順序比較ファンクタ
	//自身の持つ更新・出力コンポーネントのリスト，および保留コンポーネント
	std::set<ComponentHandle<Component>, ComponentHandleCompare> mUpdateComponents;
	std::vector<ComponentHandle<Component>> mPandingUpdateComponents;
	std::set<ComponentHandle<Component>, ComponentHandleCompare> mOutputComponents;
	std::vector<ComponentHandle<Component>> mPandingOutputComponents;
	//自分の持つ全更新・出力コンポーネントのUpdateを呼び出す(保留コンポーネントのそれは実行しない)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	//コンポーネント・オブジェクトを直接リストに入れられるか?
	bool mIsObjCompAddable;
	//Deleteフラグが立っているコンポーネント・オブジェクトや保留中のそれらの処理
	void DeleteAndProcessPandingObjComp();
	GameObject* operator&() const noexcept;
};