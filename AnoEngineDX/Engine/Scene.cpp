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
#include "Log.h"

void Scene::InitMemory()
{
	Log::OutputTrivial("Scene::InitMemory()");
	ElementContainer::MemoryInit();
}

Scene::Scene(Game* const game)
	:game_(*game), is_objcomp_addable_(true), input_system_(nullptr), prev_mouse_pos_(MatVec::Vector2(0, 0)), input_flag_(true), input_flag_for_comps_(true), update_flag_for_comps_(true), delete_check_(false), is_executing_destructor_(false)
{
	BOOST_ASSERT(game != nullptr);
}

void Scene::AsyncInitialize()
{
}

void Scene::Update(const InputSystem* input)
{
	input_system_ = input;
	//ここからしばらくの間，追加されるオブジェクト・コンポーネントは保留に入れる
	is_objcomp_addable_ = false;
	element_container_.CreateCompInitThread();
	PriorUniqueUpdate();
	//UIScreenにブロックされてなければUpdateを実行
	if (update_flag_for_comps_)
	{
		input_flag_ = input_flag_for_comps_;
		element_container_.LaunchUpdateComponents();
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
	element_container_.LaunchOutputComponents();
	LaunchOutputUIScreens();
	PosteriorUniqueOutput();
	element_container_.FinishCompInitThread();
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

std::weak_ptr<GameObject> Scene::AddObject()
{
	//デストラクタ実行中なので追加を行わない
	if (is_executing_destructor_) {
		return std::weak_ptr<GameObject>();
	}
	return element_container_.AddObject(this);
}

void Scene::SetOutputCompsPreFunc(int upd_prio, std::function<void()> func)
{
	element_container_.SetOutputCompsPreFunc(upd_prio, func);
}

void Scene::SetOutputCompsPostFunc(int upd_prio, std::function<void()> func)
{
	element_container_.SetOutputCompsPostFunc(upd_prio, func);
}

Scene::~Scene() {
	is_executing_destructor_ = true;
	element_container_.FreeAllElements();
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

void Scene::Erase(std::weak_ptr<Component> ptr)
{
	element_container_.Erase(ptr);
}

int Scene::GetGameObjectNumber()
{
	return element_container_.GetGameObjectNumber();
}

int Scene::GetUpdateComponentNumber()
{
	return element_container_.GetUpdateComponentNumber();
}

int Scene::GetOutputComponentNumber()
{
	return element_container_.GetOutputComponentNumber();
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

void Scene::ProcessPandingComps()
{
	element_container_.ProcessPandingElements();
}

void Scene::ProcessPandingUIScreens()
{
	for (int n = 0; n < panding_uiscreens_.size(); n++) {
		uiscreens_.push_back(panding_uiscreens_[n]);
		prev_mouse_pos_for_uiscreens_.push_back(GetMouseScreenPos());
		update_flag_for_uiscreens_.push_back(true);
		input_flag_for_uiscreens_.push_back(true);
	}
	panding_uiscreens_.clear();
}

