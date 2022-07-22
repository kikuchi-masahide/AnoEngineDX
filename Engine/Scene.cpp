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

//このサイズが<=64ならば1,<=96ならば2,<=128ならば3,>128ならば4を返す
int GetSizeClass(std::size_t size) {
	if (size <= 64) {
		return 1;
	}
	else if (size <= 96) {
		return 2;
	}
	else if (size <= 128) {
		return 3;
	}
	else {
		return 4;
	}
}

void Scene::InitMemory()
{
	Log::OutputTrivial("Scene::InitMemory()");
	obj_pool_.emplace(sizeof(GameObject), kMaxObjNum_);
	comp_pool_64_.emplace(64, kMaxCompNum64_);
	comp_pool_96_.emplace(96, kMaxCompNum96_);
	comp_pool_128_.emplace(128, kMaxCompNum128_);
}

Scene::Scene(Game* const game)
	:game_(*game), is_objcomp_addable_(true), input_system_(nullptr), prev_mouse_pos_(MatVec::Vector2(0, 0)), input_flag_(true), input_flag_for_comps_(true), update_flag_for_comps_(true), delete_check_(false), is_executing_destructor_(false),next_obj_id_(0)
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
	LaunchOutputUIScreens();
	PosteriorUniqueOutput();
	game_.dx12_.ProcessCommands();
	//保留していたオブジェクト・コンポーネントの処理を行う
	//TODO:本格的なオブジェクトの初期化をOutputの後に行うようにしたいので、ProcessPanding->Delete~の順にする
	ProcessPandingComps();
	DeleteUIScreen();
	is_objcomp_addable_ = true;
	ProcessPandingUIScreens();
}

void Scene::PriorUniqueOutput()
{
}

void Scene::PosteriorUniqueOutput()
{
}

GameObjectHandle Scene::AddObject(int comphandle_reserve_num)
{
	//デストラクタ実行中なので追加を行わない
	if (is_executing_destructor_) {
		return -1;
	}
	if (id_objpointer_map_.size() >= kMaxObjNum_) {
		Log::OutputCritical("Object number exceeded limit");
	}
	GameObject* objp = new(obj_pool_->malloc()) GameObject(next_obj_id_, comphandle_reserve_num);
	id_objpointer_map_.emplace(next_obj_id_, objp);
	return next_obj_id_++;
}

bool Scene::GetDeleteFlag() const
{
	return delete_flag_;
}

void Scene::SetDeleteFlag()
{
	delete_flag_ = true;
}

Scene::~Scene() {
	is_executing_destructor_ = true;
	for (auto& p : id_objpointer_map_) {
		p.second->~GameObject();
		obj_pool_->free(p.second);
	}
	id_objpointer_map_.clear();
	for (auto& comp : update_components_) {
		comp.reset();
	}
	for (auto itr = panding_update_components_.begin(); itr != panding_update_components_.end(); itr++) {
		itr->reset();
	}
	for (auto& comp : output_components_) {
		comp.reset();
	}
	for (auto itr = panding_output_components_.begin(); itr != panding_output_components_.end(); itr++) {
		itr->reset();
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

void Scene::Erase(GameObjectHandle handle)
{
	erase_objs_.push_back(handle);
}

int Scene::GetGameObjectNumber()
{
	return id_objpointer_map_.size();
}

int Scene::GetUpdateComponentNumber()
{
	return update_components_.size();
}

int Scene::GetOutputComponentNumber()
{
	return output_components_.size();
}

void Scene::LaunchUpdateComponents()
{
	for (auto itr = update_components_.begin(); itr != update_components_.end(); itr++) {
		(*itr)->Update();
	}
}

void Scene::LaunchOutputComponents()
{
	for (auto itr = output_components_.begin(); itr != output_components_.end(); itr++) {
		(*itr)->Update();
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

void Scene::ProcessPandingComps()
{
	//Component削除により子Componentのvectorのeraseを行うべきGameObject
	static std::set<GameObjectHandle> objs_to_update_childs;
	while (true) {
		//このループ内でGameObject/Componentの変更が生じたか
		//HACK:要再考
		bool has_changed = false;
		if (panding_update_components_.size() > 0) {
			for (auto itr = panding_update_components_.begin(); itr != panding_update_components_.end(); itr++) {
				(*itr)->Initialize();
				update_components_.push_back(*itr);
			}
			panding_update_components_.clear();
			has_changed = true;
		}
		if (panding_output_components_.size() > 0) {
			for (auto itr = panding_output_components_.begin(); itr != panding_output_components_.end(); itr++) {
				(*itr)->Initialize();
				output_components_.push_back(*itr);
			}
			panding_output_components_.clear();
			has_changed = true;
		}
		if (erase_objs_.size() > 0) {
			for (GameObjectHandle handle : erase_objs_) {
				auto itr = id_objpointer_map_.find(handle);
				if (itr != id_objpointer_map_.end()) {
					itr->second->SetAllCompsFlag();
					itr->second->~GameObject();
					obj_pool_->free(itr->second);
					id_objpointer_map_.erase(itr);
				}
			}
			erase_objs_.clear();
			has_changed = true;
		}
		std::erase_if(update_components_, [&](std::shared_ptr<Component>& comp) {
			if (comp->GetDeleteFlag()) {
				objs_to_update_childs.insert(comp->obj_);
				comp.reset();
				has_changed = true;
				return true;
			}
			return false;
		});
		std::sort(update_components_.begin(), update_components_.end(), 
			[](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
			return a->upd_priority_ > b->upd_priority_;
		});
		std::erase_if(output_components_, [&](std::shared_ptr<Component>& comp) {
			if (comp->GetDeleteFlag()) {
				objs_to_update_childs.insert(comp->obj_);
				comp.reset();
				has_changed = true;
				return true;
			}
			return false;
		});
		for (GameObjectHandle obj : objs_to_update_childs) {
			auto itr = id_objpointer_map_.find(obj);
			if (itr != id_objpointer_map_.end()) {
				itr->second->UnregisterInvalidChilds();
			}
		}
		objs_to_update_childs.clear();
		std::sort(output_components_.begin(), output_components_.end(),
			[](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
			return a->upd_priority_ > b->upd_priority_;
		});
		if (!has_changed) {
			break;
		}
	}
}

void Scene::ProcessPandingUIScreens()
{
	//PandingにあるUIScreenの追加
	for (int n = 0; n < panding_uiscreens_.size(); n++) {
		uiscreens_.push_back(panding_uiscreens_[n]);
		prev_mouse_pos_for_uiscreens_.push_back(GetMouseScreenPos());
		update_flag_for_uiscreens_.push_back(true);
		input_flag_for_uiscreens_.push_back(true);
	}
	panding_uiscreens_.clear();
}

std::optional<boost::pool<>> Scene::obj_pool_;
std::map<GameObjectHandle, GameObject*> Scene::id_objpointer_map_;
std::optional<boost::pool<>> Scene::comp_pool_64_;
std::optional<boost::pool<>> Scene::comp_pool_96_;
std::optional<boost::pool<>> Scene::comp_pool_128_;
int Scene::comp_pool_used_chunk_64_ = 0;
int Scene::comp_pool_used_chunk_96_ = 0;
int Scene::comp_pool_used_chunk_128_ = 0;
