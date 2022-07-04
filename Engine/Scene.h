#pragma once

#include "GameObject.h"
#include "Layer.h"
#include "InputSystem.h"
#include "Math/Math.h"

class Game;
class UIScreen;
class InputSystem;

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
	void Update(InputSystem* _input);
	/// <summary>
	/// GameObjectやUIScreenや更新処理の前に実行される，override可能な独自処理
	/// </summary>
	virtual void PriorUniqueUpdate();
	/// <summary>
	/// GameObjectやUIScreenや更新処理の後に実行される，override可能な独自処理
	/// </summary>
	virtual void PosteriorUniqueUpdate();
	/// <summary>
	/// 出力関数 処理詳細:
	/// OutputComponentsのUpdate()実行
	/// Layerの変更反映とDraw()実行
	/// UniqueOutput()
	/// DeleteFlagの立つGameObject，Component，Layerの削除と保留していたものの追加
	/// </summary>
	void Output();
	/// <summary>
	/// GameObject等のUpdateの前に実行される，override可能な独自処理
	/// </summary>
	virtual void PriorUniqueOutput();
	/// <summary>
	/// GameObject等のUpdateの後に実行される，override可能な独自処理
	/// (このフレームでのGameObject，Component追加はまだ保留状態)
	/// </summary>
	virtual void PosteriorUniqueOutput();
	GameObjectHandle AddObject(MatVec::Vector2 _pos, double _scale, double _angle);
	Game &mGame;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	//このシーンに更新・出力コンポーネントを追加する
	//GameObject::AddUpdate・OutputComponentから呼び出される
	void AddUpdateComponent(GameObject* _obj, ComponentHandle<Component> _handle);
	//このシーンに更新・出力コンポーネントを追加する
	//GameObject::AddUpdate・OutputComponentから呼び出される
	void AddOutputComponent(GameObject* _obj, ComponentHandle<Component> _handle);
	template<class T, class... Args>
	LayerHandle<T> AddLayer(Args... _args)
	{
		if (is_executing_destructor_)
		{
			return LayerHandle<T>();
		}
		T* layerp= new T(_args...);
		//直接追加してよいならばそうする
		if (mIsObjCompAddable)mLayers.insert(layerp);
		else mPandingLayers.insert(layerp);
		return layerp->This<T>();
	}
	/// <summary>
	/// T型UI画面の追加
	/// </summary>
	/// <param name="..._args">コンストラクタに渡す引数</param>
	template<class T,class... Args>
	T* AddUIScreen(Args... _args)
	{
		if (is_executing_destructor_)
		{
			return nullptr;
		}
		if (mIsObjCompAddable)
		{
			if (mUIScreens.size() == 0)
			{
				mPrevMousePosForUIScreens.push_back(mPrevMousePosForComps);
			}
			else
			{
				mPrevMousePosForUIScreens.push_back(mPrevMousePosForUIScreens.back());
			}
			auto ptr = new T(this, _args...);
			mUIScreens.push_back(ptr);
			mUpdateFlagForUIScreens.push_back(true);
			mInputFlagForUIScreens.push_back(true);
			return ptr;
		}
		else
		{
			auto ptr = new T(this, _args...);
			mPandingUIScreens.push_back(ptr);
			return ptr;
		}
	}
	/// <summary>
	///各キー・マウスのボタンの状態を受け取る
	/// </summary>
	ButtonState GetKeyState(unsigned char _key);
	/// <summary>
	//マウスのクライアント座標を得る(左下原点)
	/// </summary>
	MatVec::Vector2 GetMouseClientPos(unsigned int _windowid);
	/// <summary>
	//マウスの移動ベクトルを得る(左下原点)
	/// </summary>
	MatVec::Vector2 GetMouseMove();
	/// <summary>
	//マウスのスクリーン座標を得る(左上原点)
	/// </summary>
	MatVec::Vector2 GetMouseScreenPos();
protected:
	bool mDeleteFlag;
	virtual ~Scene();
private:
	friend Game;
	bool mDeleteCheck;
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle<Component>& left, const ComponentHandle<Component>& right) const {
			if (!left.IsValid())return false;
			if (!right.IsValid())return true;
			return left->mUpdPriority > right->mUpdPriority;
		}
	};

	class LayerCompare {
	public:
		bool operator()(const Layer* left, const Layer* right) const {
			return left->GetZ() < right->GetZ();
		}
	};

	//自身の持つGameObjectのリスト及び保留中のオブジェクト
	std::list<GameObject*> mObjs;
	std::vector<GameObject*> mPandingObjs;
	//コンポーネントを持つsetのための順序比較ファンクタ
	//自身の持つ更新・出力コンポーネントのリスト，および保留コンポーネント
	std::multiset<ComponentHandle<Component>, ComponentHandleCompare> mUpdateComponents;
	std::vector<ComponentHandle<Component>> mPandingUpdateComponents;
	std::multiset<ComponentHandle<Component>, ComponentHandleCompare> mOutputComponents;
	std::vector<ComponentHandle<Component>> mPandingOutputComponents;
	//自分の持つ全更新・出力コンポーネントのUpdateを呼び出す(保留コンポーネントのそれは実行しない)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	//コンポーネント・オブジェクトを直接リストに入れられるか?
	bool mIsObjCompAddable;
	//Deleteフラグが立っているコンポーネント・オブジェクトを削除
	void DeleteObjComp();
	GameObject* operator&() const noexcept;
	//Z座標昇順で取り出す(右手系!)
	std::multiset<Layer*, LayerCompare> mLayers;
	std::multiset<Layer*, LayerCompare> mPandingLayers;
	//自分の持つLayerのOutputを行う
	void OutputLayer();
	//DeleteFlag立ってるLayerの処理
	void DeleteLayers();
	//持っているUIScreen群(添え字の大きいものが後に追加されたUIScreen)
	std::vector<UIScreen*> mUIScreens;
	//保留UIScreen
	std::vector<UIScreen*> mPandingUIScreens;
	InputSystem* input_system_;
	//今tick，Componentに入力情報を渡すか否か
	bool mInputFlagForComps;
	//今tick，対応UIScreenに入力情報を渡すか否か
	std::vector<bool> mInputFlagForUIScreens;
	//今tick，ComponentのUpdateを実行するか否か
	bool mUpdateFlagForComps;
	//今tick，対応UIScreenのUpdateを実行するか否か
	std::vector<bool> mUpdateFlagForUIScreens;
	//GetButtonState等で使う，入力情報を渡すか否かのフラグ
	bool mInputFlag;
	//GetClientMousePos等で使う，前tickでのマウス位置
	MatVec::Vector2 mPrevMousePos;
	//Compsにとっての前tickマウス位置(左上原点スクリーン座標)
	MatVec::Vector2 mPrevMousePosForComps;
	//UIScreenにとっての前tickマウス位置(左上原点スクリーン座標)
	std::vector<MatVec::Vector2> mPrevMousePosForUIScreens;
	void DeleteUIScreen();
	//UIScreenのUpdateを奥から呼び出す
	void LaunchUIScreenUpdate();
	//UIScreenのOutputを奥から呼び出す
	void LaunchOutputUIScreens();
	//このオブジェクトのポインタをdeleteしデストラクタを呼ぶ
	void DeleteObject(GameObject* _object);
	//このレイヤーのポインタをdeleteしデストラクタを呼ぶ
	void DeleteLayer(Layer* _layer);
	//保留中のオブジェクト等をマージ
	void ProcessPandings();
	//デストラクタ実行時のみtrue
	bool is_executing_destructor_;
};