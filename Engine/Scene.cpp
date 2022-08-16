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

void Scene::Update(const InputSystem* input)
{
	input_system_ = input;
	//�������炵�΂炭�̊ԁC�ǉ������I�u�W�F�N�g�E�R���|�[�l���g�͕ۗ��ɓ����
	is_objcomp_addable_ = false;
	element_container_.CreateCompInitThread();
	PriorUniqueUpdate();
	//UIScreen�Ƀu���b�N����ĂȂ����Update�����s
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
	game_.dx12_.ProcessCommands();
	//�ۗ����Ă����I�u�W�F�N�g�E�R���|�[�l���g�̏������s��
	//TODO:�{�i�I�ȃI�u�W�F�N�g�̏�������Output�̌�ɍs���悤�ɂ������̂ŁAProcessPanding->Delete~�̏��ɂ���
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

GameObjectHandle Scene::AddObject()
{
	//�f�X�g���N�^���s���Ȃ̂Œǉ����s��Ȃ�
	if (is_executing_destructor_) {
		return GameObjectHandle();
	}
	return element_container_.AddObject(this);
}

bool Scene::GetDeleteFlag() const
{
	return delete_flag_;
}

void Scene::SetDeleteFlag()
{
	delete_flag_ = true;
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
	//UIScreen�̍폜����
	for (auto uiscreen : uiscreens_)
	{
		delete uiscreen;
	}
	for (auto uiscreen : panding_uiscreens_)
	{
		delete uiscreen;
	}
	Log::OutputTrivial("Scene::~Scene()");
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
	//���㌴�_
	MatVec::Vector2 screenpos = GetMouseScreenPos();
	//�X�N���[�����W
	POINT mousepos;
	mousepos.x = screenpos(0);
	mousepos.y = screenpos(1);
	auto window = game_.GetWindow(windowid);
	auto hwnd = window->GetWindowHandle();
	//�N���C�A���g���W�ɕϊ�
	ScreenToClient(hwnd, &mousepos);
	//�E�B���h�E�̍��������
	auto windowheight = window->GetWindowSize().bottom - window->GetWindowSize().top;
	//�������_�ɕϊ�
	mousepos.y = windowheight - mousepos.y;
	return MatVec::Vector2(mousepos.x, mousepos.y);
}

MatVec::Vector2 Scene::GetMouseMove() const
{
	if (input_flag_) {
		//�X�N���[�����W(���㌴�_)
		MatVec::Vector2 vec = input_system_->GetMouseScreenPos() - prev_mouse_pos_;
		//�������_�ɏC��
		vec(1) *= -1;
		return vec;
	}
	else {
		//���͂�Ԃ������Ȃ��ꍇ�ړ������ŕԂ�
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

void Scene::Erase(std::weak_ptr<GameObject> ptr)
{
	element_container_.Erase(ptr);
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
	//�Y�������傫�������猩�ď����ׂ�UIScreen���폜
	for (; n >= 0; n--) {
		if (uiscreens_[n]->GetDeleteFlag() == false)break;
		delete uiscreens_[n];
		uiscreens_.erase(uiscreens_.begin() + n);
		prev_mouse_pos_for_uiscreens_.erase(prev_mouse_pos_for_uiscreens_.begin() + n);
		input_flag_for_uiscreens_.erase(input_flag_for_uiscreens_.begin() + n);
		update_flag_for_uiscreens_.erase(update_flag_for_uiscreens_.begin() + n);
	}
	//mMousePosFor~�̍X�V
	for (n = 0; n < uiscreens_.size(); n++) {
		if (input_flag_for_uiscreens_[n]) {
			prev_mouse_pos_for_uiscreens_[n] = GetMouseScreenPos();
		}
	}
	if (input_flag_for_comps_) {
		prev_mouse_pos_for_comps_ = GetMouseScreenPos();
	}
	//mInput/UpdateFlagFor~�̍X�V
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
		//����UIScreen��Update�����s����Ȃ�΁CInput�̎󂯎��ۂ�ݒ肵��Update���Ăяo��
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
	//Panding�ɂ���UIScreen�̒ǉ�
	for (int n = 0; n < panding_uiscreens_.size(); n++) {
		uiscreens_.push_back(panding_uiscreens_[n]);
		prev_mouse_pos_for_uiscreens_.push_back(GetMouseScreenPos());
		update_flag_for_uiscreens_.push_back(true);
		input_flag_for_uiscreens_.push_back(true);
	}
	panding_uiscreens_.clear();
}

