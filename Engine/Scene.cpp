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
	//�������炵�΂炭�̊ԁC�ǉ������I�u�W�F�N�g�E�R���|�[�l���g�͕ۗ��ɓ����
	is_objcomp_addable_ = false;
	PriorUniqueUpdate();
	//UIScreen�Ƀu���b�N����ĂȂ����Update�����s
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
	//�ۗ����Ă����I�u�W�F�N�g�E�R���|�[�l���g�̏������s��
	//TODO:�{�i�I�ȃI�u�W�F�N�g�̏�������Output�̌�ɍs���悤�ɂ������̂ŁAProcessPanding->Delete~�̏��ɂ���
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
	//�f�X�g���N�^���s���ɌĂяo���ꂽ�牽�����Ȃ�
	if (is_executing_destructor_)
	{
		return GameObjectHandle();
	}
	//�ǉ�����I�u�W�F�N�g
	GameObject* objp(new GameObject(this, pos, scale, angle));
	//���ڒǉ����Ă悢�Ȃ�΂�������
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
	//HACK:���̊֐����ŃI�u�W�F�N�g�Ȃǂ̃f�X�g���N�^���Ăяo���Ă���Œ��ɁCAddObject�Ȃǂ��Ăяo�����ƁC
	//�ǂ����g��Ȃ��̂ɃC���X�^���X���������邱�ƂɂȂ�̂ɉ����C
	//�V�����C���X�^���X�̃f�X�g���N�^���炳��ɐV�����C���X�^���X���ł�����ƁC���Ȃ�ʓ|�Ȃ��Ƃ�
	//�Ȃ肻���Ȃ̂ŁCis_executing_destructor_����ł͐������~�߂�悤�Ɍ��󂵂Ă���D
	//���l�̂��Ƃ�Game�ł��s���Ă���D
	//�������̏ꍇ�CAddObject������Ԃ����n���h����valid���m���߂Ȃ��Ă͂����Ȃ��Ȃ�̂���_�D
	//�I�u�W�F�N�g�̃f�X�g���N�^�����s����O�̏I��������2��ނɕ�����Ή������邩������Ȃ����C
	//���󂻂̃`�F�b�N�@�\���݂����Ȃ����ߌ�����
	is_executing_destructor_ = true;
	//GameObject�̍폜����
	for (auto object : objs_)
	{
		DeleteObject(object);
	}
	for (auto object : panding_objs_)
	{
		DeleteObject(object);
	}
	//Layer�̍폜����
	for (auto layer : layers_)
	{
		DeleteLayer(layer);
	}
	for (auto layer : panding_layers_)
	{
		DeleteLayer(layer);
	}
	//UIScreen�̍폜����
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

void Scene::LaunchUpdateComponents()
{
	for (auto itr = update_components_.begin(); itr != update_components_.end();) {
		//�Otick��component�폜�Ŗ����w���Ă���n���h�����폜
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
	//�S�I�u�W�F�N�g�����
	auto objitr = objs_.begin();
	while (objitr != objs_.end()) {
		//���̃I�u�W�F�N�g�̃t���O�������Ă���Ȃ�Ώ���
		if ((*objitr)->GetDeleteFlag()) {
			DeleteObject(*objitr);
			objitr = objs_.erase(objitr);
		}
		else {
			//�I�u�W�F�N�g�ɂ���Ȃ��R���|�[�l���g���폜������
			(*objitr)->DeleteFlagedComponents();
			objitr++;
		}
	}
}

void Scene::OutputLayer()
{
	//z�̕ύX��������Layer�����������C�����łȂ�����Rect���X�V
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
	//z�ύX����Layer���t���b�V�����CmLayers�ɖ߂�
	itr = zchanged.begin();
	while (itr != zchanged.end()) {
		(*itr)->FlushZRectChange(*itr);
		layers_.insert(*itr);
		itr++;
	}
	//mLayers��Output���Ăяo��
	itr = layers_.begin();
	while (itr != layers_.end()) {
		(*itr)->Draw();
		itr++;
	}
}

void Scene::DeleteLayers()
{
	//Delete�t���O���Ă�Layer���폜
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
	//�ۗ����Ă����I�u�W�F�N�g�E�R���|�[�l���g��ǉ�
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

	//Panding��Layer���t���b�V����mLayer�ɒǉ�
	auto itr = panding_layers_.begin();
	while (itr != panding_layers_.end()) {
		(*itr)->FlushZRectChange(*itr);
		layers_.insert(*itr);
		itr++;
	}
	panding_layers_.clear();

	//Panding�ɂ���UIScreen�̒ǉ�
	for (int n = 0; n < panding_uiscreens_.size(); n++) {
		uiscreens_.push_back(panding_uiscreens_[n]);
		prev_mouse_pos_for_uiscreens_.push_back(GetMouseScreenPos());
		update_flag_for_uiscreens_.push_back(true);
		input_flag_for_uiscreens_.push_back(true);
	}
	panding_uiscreens_.clear();
}