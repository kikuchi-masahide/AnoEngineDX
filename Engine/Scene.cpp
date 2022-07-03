#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "UIScreen.h"
#include "Game.h"
#include "window.h"

Scene::Scene(Game* _game)
	:mGame(*_game), mIsObjCompAddable(true), mInputSystem(nullptr), mPrevMousePos(MatVec::Vector2(0, 0)), mInputFlag(true), mInputFlagForComps(true), mUpdateFlagForComps(true), mDeleteCheck(false),is_executing_destructor_(false)
{
	BOOST_ASSERT(_game != nullptr);
}

void Scene::Update(InputSystem* _input)
{
	auto start = timeGetTime();
	auto end = start;
	mInputSystem = _input;
	//ここからしばらくの間，追加されるオブジェクト・コンポーネントは保留に入れる
	mIsObjCompAddable = false;
	//優先度の高い，独自更新処理
	PriorUniqueUpdate();
	//UIScreenにブロックされてなければUpdateを実行
	if (mUpdateFlagForComps)
	{
		mInputFlag = mInputFlagForComps;
		LaunchUpdateComponents();
	}
	LaunchUIScreenUpdate();
	PosteriorUniqueUpdate();
}

void Scene::PriorUniqueUpdate()
{
}

void Scene::PosteriorUniqueUpdate()
{
}

void Scene::Output()
{
	auto start = timeGetTime();
	auto end = start;
	PriorUniqueOutput();
	LaunchOutputComponents();
	OutputLayer();
	LaunchOutputUIScreens();
	PosteriorUniqueOutput();
	mGame.mdx12.ProcessCommands();
	//保留していたオブジェクト・コンポーネントの処理を行う
	DeleteObjComp();
	DeleteLayers();
	DeleteUIScreen();
	mIsObjCompAddable = true;
	ProcessPandings();
}

void Scene::PriorUniqueOutput()
{
}

void Scene::PosteriorUniqueOutput()
{
}

GameObjectHandle Scene::AddObject(MatVec::Vector2 _pos, double _scale, double _angle)
{
	//デストラクタ実行中に呼び出されたら何もしない
	if (is_executing_destructor_)
	{
		return GameObjectHandle();
	}
	//追加するオブジェクト
	GameObject* objp(new GameObject(this, _pos, _scale, _angle));
	//直接追加してよいならばそうする
	if (mIsObjCompAddable)mObjs.push_back(objp);
	else mPandingObjs.push_back(objp);
	return objp->This();
}

void Scene::AddUpdateComponent(GameObject* _obj, ComponentHandle<Component> _handle)
{
	BOOST_ASSERT(_obj != nullptr);
	if (mIsObjCompAddable)mUpdateComponents.insert(_handle);
	else mPandingUpdateComponents.push_back(_handle);
}

void Scene::AddOutputComponent(GameObject* _obj, ComponentHandle<Component> _handle)
{
	BOOST_ASSERT(_obj != nullptr);
	if (mIsObjCompAddable) {
		mOutputComponents.insert(_handle);
	}
	else
	{
		mPandingOutputComponents.push_back(_handle);
	}
}

Scene::~Scene() {
	BOOST_ASSERT_MSG(mDeleteCheck == true, "irregal destructor call without Game permission");
	//HACK:この関数内でオブジェクトなどのデストラクタを呼び出している最中に，AddObjectなどを呼び出されると，
	//どうせ使わないのにインスタンス生成をすることになるのに加え，
	//新しいインスタンスのデストラクタからさらに新しいインスタンスができたりと，かなり面倒なことに
	//なりそうなので，is_executing_destructor_次第では生成を止めるように現状している．
	//同様のことはGameでも行っている．
	//ただこの場合，AddObject等から返されるハンドルがvalidか確かめなくてはいけなくなるのが難点．
	//オブジェクトのデストラクタを実行する前の終了処理を2種類に分ければ解決するかもしれないが，
	//現状そのチェック機構が設けられないため見送り
	is_executing_destructor_ = true;
	//GameObjectの削除処理
	for (auto object : mObjs)
	{
		DeleteObject(object);
	}
	for (auto object : mPandingObjs)
	{
		DeleteObject(object);
	}
	//Layerの削除処理
	for (auto layer : mLayers)
	{
		DeleteLayer(layer);
	}
	for (auto layer : mPandingLayers)
	{
		DeleteLayer(layer);
	}
	//UIScreenの削除処理
	for (auto uiscreen : mUIScreens)
	{
		delete uiscreen;
	}
	for (auto uiscreen : mPandingUIScreens)
	{
		delete uiscreen;
	}
}

ButtonState Scene::GetKeyState(unsigned char _key)
{
	if (mInputFlag)
	{
		return mInputSystem->GetKeyState(_key);
	}
	else
	{
		return ButtonState::None;
	}
}

MatVec::Vector2 Scene::GetMouseClientPos(unsigned int _windowid)
{
	//左上原点
	MatVec::Vector2 screenpos = GetMouseScreenPos();
	//スクリーン座標
	POINT mousepos;
	mousepos.x = screenpos(0);
	mousepos.y = screenpos(1);
	auto window = mGame.GetWindow(_windowid);
	auto hwnd = window->GetWindowHandle();
	//クライアント座標に変換
	ScreenToClient(hwnd, &mousepos);
	//ウィンドウの高さを入手
	auto windowheight = window->GetWindowSize().bottom - window->GetWindowSize().top;
	//左下原点に変換
	mousepos.y = windowheight - mousepos.y;
	return MatVec::Vector2(mousepos.x, mousepos.y);
}

MatVec::Vector2 Scene::GetMouseMove()
{
	if (mInputFlag)
	{
		//スクリーン座標(左上原点)
		MatVec::Vector2 vec = mInputSystem->GetMouseScreenPos() - mPrevMousePos;
		//左下原点に修正
		vec(1) *= -1;
		return vec;
	}
	else
	{
		//入力を返したくない場合移動無しで返す
		return MatVec::Vector2(0, 0);
	}
}

MatVec::Vector2 Scene::GetMouseScreenPos()
{
	if (mInputFlag)
	{
		return mInputSystem->GetMouseScreenPos();
	}
	else
	{
		return mPrevMousePos;
	}
}

void Scene::LaunchUpdateComponents()
{
	for (auto itr = mUpdateComponents.begin(); itr != mUpdateComponents.end();)
	{
		//前tickのcomponent削除で無を指しているハンドルを削除
		if (!(itr->IsValid()))
		{
			itr = mUpdateComponents.erase(itr);
		}
		else {
			(*itr)->Update();
			itr++;
		}
	}
}

void Scene::LaunchOutputComponents()
{
	for (auto itr = mOutputComponents.begin(); itr != mOutputComponents.end();)
	{
		if (!(itr->IsValid()))
		{
			itr = mOutputComponents.erase(itr);
		}
		else {
			(*itr)->Update();
			itr++;
		}
	}
}

void Scene::DeleteObjComp()
{
	//全オブジェクトを回る
	auto objitr = mObjs.begin();
	while (objitr != mObjs.end()) {
		//そのオブジェクトのフラグが立っているならば消去
		if ((*objitr)->GetDeleteFlag()) {
			DeleteObject(*objitr);
			objitr = mObjs.erase(objitr);
		}
		else {
			//オブジェクトにいらないコンポーネントを削除させる
			(*objitr)->DeleteFlagedComponents();
			objitr++;
		}
	}
}

void Scene::OutputLayer()
{
	//zの変更があったLayerを引き抜き，そうでない元はRectを更新
	std::set<Layer*, LayerCompare> zchanged;
	auto itr = mLayers.begin();
	while (itr != mLayers.end())
	{
		if ((*itr)->HasZChanged())
		{
			zchanged.insert(*itr);
			itr = mLayers.erase(itr);
		}
		else {
			(*itr)->FlushZRectChange(*itr);
			itr++;
		}
	}
	//z変更したLayerをフラッシュし，mLayersに戻す
	itr = zchanged.begin();
	while (itr != zchanged.end())
	{
		(*itr)->FlushZRectChange(*itr);
		mLayers.insert(*itr);
		itr++;
	}
	//mLayersのOutputを呼び出す
	itr = mLayers.begin();
	while (itr != mLayers.end())
	{
		(*itr)->Draw();
		itr++;
	}
}

void Scene::DeleteLayers()
{
	//DeleteフラグついてるLayerを削除
	auto itr = mLayers.begin();
	while (itr != mLayers.end())
	{
		if ((*itr)->GetDeleteFlag())
		{
			DeleteLayer(*itr);
			mLayers.erase(*itr);
		}
		else {
			itr++;
		}
	}
}

void Scene::DeleteUIScreen()
{
	int n = mUIScreens.size() - 1;
	//添え字が大きい方から見て消すべきUIScreenを削除
	for (; n >= 0; n--)
	{
		if (mUIScreens[n]->GetDeleteFlag() == false)break;
		delete mUIScreens[n];
		mUIScreens.erase(mUIScreens.begin() + n);
		mPrevMousePosForUIScreens.erase(mPrevMousePosForUIScreens.begin() + n);
		mInputFlagForUIScreens.erase(mInputFlagForUIScreens.begin() + n);
		mUpdateFlagForUIScreens.erase(mUpdateFlagForUIScreens.begin() + n);
	}
	//mMousePosFor~の更新
	for (n = 0; n < mUIScreens.size(); n++)
	{
		if (mInputFlagForUIScreens[n])
		{
			mPrevMousePosForUIScreens[n] = GetMouseScreenPos();
		}
	}
	if (mInputFlagForComps)
	{
		mPrevMousePosForComps = GetMouseScreenPos();
	}
	//mInput/UpdateFlagFor~の更新
	if (mUIScreens.size() == 0)
	{
		mInputFlagForComps = true;
		mUpdateFlagForComps = true;
	}
	else
	{
		mInputFlagForUIScreens.back() = true;
		for (n = mUIScreens.size() - 2; n >= 0; n--)
		{
			mInputFlagForUIScreens[n] = mInputFlagForUIScreens[n + 1];
			if (mInputFlagForUIScreens[n] &&
				(mUIScreens[n + 1]->DoesInputGoThrough() == false)
				)
			{
				mInputFlagForUIScreens[n] = false;
			}
		}
		mInputFlagForComps = mInputFlagForUIScreens[0];
		if (mInputFlagForComps &&
			(mUIScreens[0]->DoesInputGoThrough() == false))
		{
			mInputFlagForComps = false;
		}
		mUpdateFlagForUIScreens.back() = true;
		for (n = mUIScreens.size() - 2; n >= 0; n--)
		{
			mUpdateFlagForUIScreens[n] = mUpdateFlagForUIScreens[n + 1];
			if (mUpdateFlagForUIScreens[n] &&
				(mUIScreens[n + 1]->DoesUpdateComp() == false))
			{
				mUpdateFlagForUIScreens[n] = false;
			}
		}
		mUpdateFlagForComps = mUpdateFlagForUIScreens[0];
		if (mUpdateFlagForComps &&
			(mUIScreens[0]->DoesUpdateComp() == false))
		{
			mUpdateFlagForComps = false;
		}
	}
}

void Scene::LaunchUIScreenUpdate()
{
	if (mUIScreens.size() == 0)return;
	for (int n = 0; n < mUIScreens.size(); n++)
	{
		//このUIScreenのUpdateを実行するならば，Inputの受け取り可否を設定してUpdateを呼び出し
		if (mUpdateFlagForUIScreens[n])
		{
			mInputFlag = mInputFlagForUIScreens[n];
			mPrevMousePos = mPrevMousePosForUIScreens[n];
			mUIScreens[n]->Update();
		}
	}
}

void Scene::LaunchOutputUIScreens()
{
	for (int n = 0; n < mUIScreens.size(); n++)
	{
		mUIScreens[n]->Output();
	}
}

void Scene::DeleteObject(GameObject* _object)
{
	delete _object;
}

void Scene::DeleteLayer(Layer* _layer)
{
	_layer->mDeleteCheck = true;
	delete _layer;
}

void Scene::ProcessPandings()
{
	//保留していたオブジェクト・コンポーネントを追加
	for (auto& obj : mPandingObjs) {
		mObjs.push_back(obj);
	}
	mPandingObjs.clear();
	for (auto& handle : mPandingUpdateComponents) {
		mUpdateComponents.insert(handle);
	}
	mPandingUpdateComponents.clear();
	for (auto& handle : mPandingOutputComponents) {
		mOutputComponents.insert(handle);
	}
	mPandingOutputComponents.clear();

	//PandingのLayerをフラッシュしmLayerに追加
	auto itr = mPandingLayers.begin();
	while (itr != mPandingLayers.end())
	{
		(*itr)->FlushZRectChange(*itr);
		mLayers.insert(*itr);
		itr++;
	}
	mPandingLayers.clear();

	//PandingにあるUIScreenの追加
	for (int n = 0; n < mPandingUIScreens.size(); n++)
	{
		mUIScreens.push_back(mPandingUIScreens[n]);
		mPrevMousePosForUIScreens.push_back(GetMouseScreenPos());
		mUpdateFlagForUIScreens.push_back(true);
		mInputFlagForUIScreens.push_back(true);
	}
	mPandingUIScreens.clear();
}
