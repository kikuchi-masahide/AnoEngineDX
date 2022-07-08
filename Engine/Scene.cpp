//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "UIScreen.h"
#include "Game.h"
#include "window.h"

Scene::Scene(Game* const game)
	:game_(*game), is_objcomp_addable_(true), input_system_(nullptr), prev_mouse_pos_(MatVec::Vector2(0, 0)), input_flag_(true), input_flag_for_comps_(true), update_flag_for_comps_(true), delete_check_(false), is_executing_destructor_(false)
{
	BOOST_ASSERT(game != nullptr);
}

void Scene::Update(const InputSystem* input)
{
	input_system_ = input;
	//ここからしばらくの間，追加されるオブジェクト・コンポーネントは保留に入れる
	is_objcomp_addable_ = false;
	PriorUniqueUpdate();
	//UIScreenにブロックされてなければUpdateを実行
	if (update_flag_for_comps_)
	{
		input_flag_ = input_flag_for_comps_;
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
	PriorUniqueOutput();
	LaunchOutputComponents();
	OutputLayer();
	LaunchOutputUIScreens();
	PosteriorUniqueOutput();
	game_.dx12_.ProcessCommands();
	//保留していたオブジェクト・コンポーネントの処理を行う
	//TODO:本格的なオブジェクトの初期化をOutputの後に行うようにしたいので、ProcessPanding->Delete~の順にする
	DeleteObjComp();
	DeleteLayers();
	DeleteUIScreen();
	is_objcomp_addable_ = true;
	ProcessPandings();
}

void Scene::PriorUniqueOutput()
{
}

void Scene::PosteriorUniqueOutput()
{
}

GameObjectHandle Scene::AddObject(MatVec::Vector2 pos, double scale, double angle)
{
	//デストラクタ実行中に呼び出されたら何もしない
	if (is_executing_destructor_)
	{
		return GameObjectHandle();
	}
	//追加するオブジェクト
	GameObject* objp(new GameObject(this, pos, scale, angle));
	//直接追加してよいならばそうする
	if (is_objcomp_addable_)objs_.push_back(objp);
	else panding_objs_.push_back(objp);
	return objp->This();
}

bool Scene::GetDeleteFlag() const
{
	return delete_flag_;
}

void Scene::SetDeleteFlag()
{
	delete_flag_ = true;
}

void Scene::AddUpdateComponent(GameObject* obj, ComponentHandle<Component> handle)
{
	BOOST_ASSERT(obj != nullptr);
	if (is_objcomp_addable_) {
		update_components_.insert(handle);
	}
	else {
		panding_update_components_.push_back(handle);
	}
}

void Scene::AddOutputComponent(GameObject* obj, ComponentHandle<Component> handle)
{
	BOOST_ASSERT(obj != nullptr);
	if (is_objcomp_addable_) {
		output_components_.insert(handle);
	}
	else {
		panding_output_components_.push_back(handle);
	}
}

Scene::~Scene() {
	BOOST_ASSERT_MSG(delete_check_ == true, "irregal destructor call without Game permission");
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
	for (auto object : objs_)
	{
		DeleteObject(object);
	}
	for (auto object : panding_objs_)
	{
		DeleteObject(object);
	}
	//Layerの削除処理
	for (auto layer : layers_)
	{
		DeleteLayer(layer);
	}
	for (auto layer : panding_layers_)
	{
		DeleteLayer(layer);
	}
	//UIScreenの削除処理
	for (auto uiscreen : uiscreens_)
	{
		delete uiscreen;
	}
	for (auto uiscreen : panding_uiscreens_)
	{
		delete uiscreen;
	}
}

ButtonState Scene::GetKeyState(int key) const
{
	if (input_flag_) {
		return input_system_->GetKeyState(key);
	}
	else {
		return ButtonState::None;
	}
}

MatVec::Vector2 Scene::GetMouseClientPos(int windowid) const
{
	//左上原点
	MatVec::Vector2 screenpos = GetMouseScreenPos();
	//スクリーン座標
	POINT mousepos;
	mousepos.x = screenpos(0);
	mousepos.y = screenpos(1);
	auto window = game_.GetWindow(windowid);
	auto hwnd = window->GetWindowHandle();
	//クライアント座標に変換
	ScreenToClient(hwnd, &mousepos);
	//ウィンドウの高さを入手
	auto windowheight = window->GetWindowSize().bottom - window->GetWindowSize().top;
	//左下原点に変換
	mousepos.y = windowheight - mousepos.y;
	return MatVec::Vector2(mousepos.x, mousepos.y);
}

MatVec::Vector2 Scene::GetMouseMove() const
{
	if (input_flag_) {
		//スクリーン座標(左上原点)
		MatVec::Vector2 vec = input_system_->GetMouseScreenPos() - prev_mouse_pos_;
		//左下原点に修正
		vec(1) *= -1;
		return vec;
	}
	else {
		//入力を返したくない場合移動無しで返す
		return MatVec::Vector2(0, 0);
	}
}

MatVec::Vector2 Scene::GetMouseScreenPos() const
{
	if (input_flag_) {
		return input_system_->GetMouseScreenPos();
	}
	else {
		return prev_mouse_pos_;
	}
}

void Scene::LaunchUpdateComponents()
{
	for (auto itr = update_components_.begin(); itr != update_components_.end();) {
		//前tickのcomponent削除で無を指しているハンドルを削除
		if (!(itr->IsValid())) {
			itr = update_components_.erase(itr);
		}
		else {
			(*itr)->Update();
			itr++;
		}
	}
}

void Scene::LaunchOutputComponents()
{
	for (auto itr = output_components_.begin(); itr != output_components_.end();) {
		if (!(itr->IsValid())) {
			itr = output_components_.erase(itr);
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
	auto objitr = objs_.begin();
	while (objitr != objs_.end()) {
		//そのオブジェクトのフラグが立っているならば消去
		if ((*objitr)->GetDeleteFlag()) {
			DeleteObject(*objitr);
			objitr = objs_.erase(objitr);
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
	auto itr = layers_.begin();
	while (itr != layers_.end()) {
		if ((*itr)->HasZChanged()) {
			zchanged.insert(*itr);
			itr = layers_.erase(itr);
		}
		else {
			(*itr)->FlushZRectChange(*itr);
			itr++;
		}
	}
	//z変更したLayerをフラッシュし，mLayersに戻す
	itr = zchanged.begin();
	while (itr != zchanged.end()) {
		(*itr)->FlushZRectChange(*itr);
		layers_.insert(*itr);
		itr++;
	}
	//mLayersのOutputを呼び出す
	itr = layers_.begin();
	while (itr != layers_.end()) {
		(*itr)->Draw();
		itr++;
	}
}

void Scene::DeleteLayers()
{
	//DeleteフラグついてるLayerを削除
	auto itr = layers_.begin();
	while (itr != layers_.end()) {
		if ((*itr)->GetDeleteFlag()) {
			DeleteLayer(*itr);
			layers_.erase(*itr);
		}
		else {
			itr++;
		}
	}
}

void Scene::DeleteUIScreen()
{
	int n = uiscreens_.size() - 1;
	//添え字が大きい方から見て消すべきUIScreenを削除
	for (; n >= 0; n--) {
		if (uiscreens_[n]->GetDeleteFlag() == false)break;
		delete uiscreens_[n];
		uiscreens_.erase(uiscreens_.begin() + n);
		prev_mouse_pos_for_uiscreens_.erase(prev_mouse_pos_for_uiscreens_.begin() + n);
		input_flag_for_uiscreens_.erase(input_flag_for_uiscreens_.begin() + n);
		update_flag_for_uiscreens_.erase(update_flag_for_uiscreens_.begin() + n);
	}
	//mMousePosFor~の更新
	for (n = 0; n < uiscreens_.size(); n++) {
		if (input_flag_for_uiscreens_[n]) {
			prev_mouse_pos_for_uiscreens_[n] = GetMouseScreenPos();
		}
	}
	if (input_flag_for_comps_) {
		prev_mouse_pos_for_comps_ = GetMouseScreenPos();
	}
	//mInput/UpdateFlagFor~の更新
	if (uiscreens_.size() == 0) {
		input_flag_for_comps_ = true;
		update_flag_for_comps_ = true;
	}
	else {
		input_flag_for_uiscreens_.back() = true;
		for (n = uiscreens_.size() - 2; n >= 0; n--) {
			input_flag_for_uiscreens_[n] = input_flag_for_uiscreens_[n + 1];
			if (input_flag_for_uiscreens_[n] &&
				(uiscreens_[n + 1]->DoesInputGoThrough() == false)) {
				input_flag_for_uiscreens_[n] = false;
			}
		}
		input_flag_for_comps_ = input_flag_for_uiscreens_[0];
		if (input_flag_for_comps_ &&
			(uiscreens_[0]->DoesInputGoThrough() == false)) {
			input_flag_for_comps_ = false;
		}
		update_flag_for_uiscreens_.back() = true;
		for (n = uiscreens_.size() - 2; n >= 0; n--) {
			update_flag_for_uiscreens_[n] = update_flag_for_uiscreens_[n + 1];
			if (update_flag_for_uiscreens_[n] &&
				(uiscreens_[n + 1]->DoesUpdateComp() == false)) {
				update_flag_for_uiscreens_[n] = false;
			}
		}
		update_flag_for_comps_ = update_flag_for_uiscreens_[0];
		if (update_flag_for_comps_ &&
			(uiscreens_[0]->DoesUpdateComp() == false)) {
			update_flag_for_comps_ = false;
		}
	}
}

void Scene::LaunchUIScreenUpdate()
{
	if (uiscreens_.size() == 0)return;
	for (int n = 0; n < uiscreens_.size(); n++) {
		//このUIScreenのUpdateを実行するならば，Inputの受け取り可否を設定してUpdateを呼び出し
		if (update_flag_for_uiscreens_[n]) {
			input_flag_ = input_flag_for_uiscreens_[n];
			prev_mouse_pos_ = prev_mouse_pos_for_uiscreens_[n];
			uiscreens_[n]->Update();
		}
	}
}

void Scene::LaunchOutputUIScreens()
{
	for (int n = 0; n < uiscreens_.size(); n++) {
		uiscreens_[n]->Output();
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
	for (auto& obj : panding_objs_) {
		objs_.push_back(obj);
	}
	panding_objs_.clear();
	for (auto& handle : panding_update_components_) {
		update_components_.insert(handle);
	}
	panding_update_components_.clear();
	for (auto& handle : panding_output_components_) {
		output_components_.insert(handle);
	}
	panding_output_components_.clear();

	//PandingのLayerをフラッシュしmLayerに追加
	auto itr = panding_layers_.begin();
	while (itr != panding_layers_.end()) {
		(*itr)->FlushZRectChange(*itr);
		layers_.insert(*itr);
		itr++;
	}
	panding_layers_.clear();

	//PandingにあるUIScreenの追加
	for (int n = 0; n < panding_uiscreens_.size(); n++) {
		uiscreens_.push_back(panding_uiscreens_[n]);
		prev_mouse_pos_for_uiscreens_.push_back(GetMouseScreenPos());
		update_flag_for_uiscreens_.push_back(true);
		input_flag_for_uiscreens_.push_back(true);
	}
	panding_uiscreens_.clear();
}