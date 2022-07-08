//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObject.h"
#include "Layer.h"
#include "InputSystem.h"
#include "Math/Math.h"

class Game;
class UIScreen;
class InputSystem;

//TODO:�R���|�[�l���g�A�I�u�W�F�N�g�̏������̕��@���l����
//�R���X�g���N�^�͏����̌y������ -> Init()���ォ��Ăяo���{�i�I�ȏ��������s��
//Init()���Ăяo���Ă��Ȃ��C���X�^���X���n���h������Ăяo���Ȃ��悤�ɂ���

/// <summary>
/// �V�[����\���N���X
/// </summary>
class Scene {
public:
	//�R���X�g���N�^���Ăяo����̂�_game���g��AddChild�̂�
	Scene(Game* const game);
	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update(const InputSystem* input);
	/// <summary>
	/// GameObject��UIScreen��X�V�����̑O�Ɏ��s�����Coverride�\�ȓƎ�����
	/// </summary>
	virtual void PriorUniqueUpdate();
	/// <summary>
	/// GameObject��UIScreen��X�V�����̌�Ɏ��s�����Coverride�\�ȓƎ�����
	/// </summary>
	virtual void PosteriorUniqueUpdate();
	/// <summary>
	/// �o�͊֐�
	/// </summary>
	void Output();
	/// <summary>
	/// GameObject����Update�̑O�Ɏ��s�����Coverride�\�ȓƎ�����
	/// </summary>
	virtual void PriorUniqueOutput();
	/// <summary>
	/// GameObject����Update�̌�Ɏ��s�����Coverride�\�ȓƎ�����
	/// (���̃t���[���ł�GameObject�CComponent�ǉ��͂܂��ۗ����)
	/// </summary>
	virtual void PosteriorUniqueOutput();
	GameObjectHandle AddObject(MatVec::Vector2 pos, double scale, double angle);
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	//���̃V�[���ɍX�V�E�o�̓R���|�[�l���g��ǉ�����
	//GameObject::AddUpdate�EOutputComponent����Ăяo�����
	void AddUpdateComponent(GameObject* obj, ComponentHandle<Component> handle);
	//���̃V�[���ɍX�V�E�o�̓R���|�[�l���g��ǉ�����
	//GameObject::AddUpdate�EOutputComponent����Ăяo�����
	void AddOutputComponent(GameObject* obj, ComponentHandle<Component> handle);
	template<class T, class... Args>
	LayerHandle<T> AddLayer(Args... _args)
	{
		if (is_executing_destructor_) {
			return LayerHandle<T>();
		}
		T* layerp = new T(_args...);
		//���ڒǉ����Ă悢�Ȃ�΂�������
		if (is_objcomp_addable_) {
			layers_.insert(layerp);
		}
		else {
			panding_layers_.insert(layerp);
		}
		return layerp->This<T>();
	}
	/// <summary>
	/// UIScreen���p������N���X�̒ǉ�
	/// </summary>
	/// <param name="..._args">�R���X�g���N�^�ɓn������</param>
	template<class T, class... Args>
	T* AddUIScreen(Args... _args)
	{
		if (is_executing_destructor_) {
			return nullptr;
		}
		if (is_objcomp_addable_) {
			if (uiscreens_.size() == 0) {
				prev_mouse_pos_for_uiscreens_.push_back(prev_mouse_pos_for_comps_);
			}
			else {
				prev_mouse_pos_for_uiscreens_.push_back(prev_mouse_pos_for_uiscreens_.back());
			}
			auto ptr = new T(this, _args...);
			uiscreens_.push_back(ptr);
			update_flag_for_uiscreens_.push_back(true);
			input_flag_for_uiscreens_.push_back(true);
			return ptr;
		}
		else {
			auto ptr = new T(this, _args...);
			panding_uiscreens_.push_back(ptr);
			return ptr;
		}
	}
	/// <summary>
	///�e�L�[�E�}�E�X�̃{�^���̏�Ԃ��󂯎��
	/// </summary>
	ButtonState GetKeyState(int _key) const;
	/// <summary>
	//�}�E�X�̃N���C�A���g���W�𓾂�(�������_)
	/// </summary>
	MatVec::Vector2 GetMouseClientPos(int windowid) const;
	/// <summary>
	//�}�E�X�̈ړ��x�N�g���𓾂�(�������_)
	/// </summary>
	MatVec::Vector2 GetMouseMove() const;
	/// <summary>
	//�}�E�X�̃X�N���[�����W�𓾂�(���㌴�_)
	/// </summary>
	MatVec::Vector2 GetMouseScreenPos() const;
	Game& game_;
protected:
	virtual ~Scene();
private:
	//�R���|�[�l���g������set�̂��߂̏�����r�t�@���N�^
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
	//�����̎��S�X�V�E�o�̓R���|�[�l���g��Update���Ăяo��(�ۗ��R���|�[�l���g�̂���͎��s���Ȃ�)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	//Delete�t���O�������Ă���R���|�[�l���g�E�I�u�W�F�N�g���폜
	void DeleteObjComp();
	//Z���W�����Ŏ��o��(�E��n!)
	std::multiset<Layer*, LayerCompare> layers_;
	std::multiset<Layer*, LayerCompare> panding_layers_;
	//�����̎���Layer��Output���s��
	void OutputLayer();
	//DeleteFlag�����Ă�Layer�̏���
	void DeleteLayers();
	void DeleteUIScreen();
	//UIScreen��Update��������Ăяo��
	void LaunchUIScreenUpdate();
	//UIScreen��Output��������Ăяo��
	void LaunchOutputUIScreens();
	//���̃I�u�W�F�N�g�̃|�C���^��delete���f�X�g���N�^���Ă�
	void DeleteObject(GameObject* _object);
	//���̃��C���[�̃|�C���^��delete���f�X�g���N�^���Ă�
	void DeleteLayer(Layer* _layer);
	//�ۗ����̃I�u�W�F�N�g�����}�[�W
	void ProcessPandings();
	GameObject* operator&() const noexcept;
	bool delete_flag_;
	friend Game;
	bool delete_check_;
	//���g�̎���GameObject�̃��X�g�y�ѕۗ����̃I�u�W�F�N�g
	std::list<GameObject*> objs_;
	std::vector<GameObject*> panding_objs_;
	//���g�̎��X�V�E�o�̓R���|�[�l���g�̃��X�g�C����ѕۗ��R���|�[�l���g
	std::multiset<ComponentHandle<Component>, ComponentHandleCompare> update_components_;
	std::vector<ComponentHandle<Component>> panding_update_components_;
	std::multiset<ComponentHandle<Component>, ComponentHandleCompare> output_components_;
	std::vector<ComponentHandle<Component>> panding_output_components_;
	//�R���|�[�l���g�E�I�u�W�F�N�g�𒼐ڃ��X�g�ɓ�����邩?
	bool is_objcomp_addable_;
	//�����Ă���UIScreen�Q(�Y�����̑傫�����̂���ɒǉ����ꂽUIScreen)
	std::vector<UIScreen*> uiscreens_;
	//�ۗ�UIScreen
	std::vector<UIScreen*> panding_uiscreens_;
	const InputSystem* input_system_;
	//��tick�CComponent�ɓ��͏���n�����ۂ�
	bool input_flag_for_comps_;
	//��tick�CComponent��Update�����s���邩�ۂ�
	bool update_flag_for_comps_;
	//��tick�C�Ή�UIScreen�ɓ��͏���n�����ۂ�
	std::vector<bool> input_flag_for_uiscreens_;
	//��tick�C�Ή�UIScreen��Update�����s���邩�ۂ�
	std::vector<bool> update_flag_for_uiscreens_;
	//GetButtonState���Ŏg���C���͏���n�����ۂ��̃t���O
	bool input_flag_;
	//GetClientMousePos���Ŏg���C�Otick�ł̃}�E�X�ʒu
	MatVec::Vector2 prev_mouse_pos_;
	//Comps�ɂƂ��Ă̑Otick�}�E�X�ʒu(���㌴�_�X�N���[�����W)
	MatVec::Vector2 prev_mouse_pos_for_comps_;
	//UIScreen�ɂƂ��Ă̑Otick�}�E�X�ʒu(���㌴�_�X�N���[�����W)
	std::vector<MatVec::Vector2> prev_mouse_pos_for_uiscreens_;
	//�f�X�g���N�^���s���̂�true
	bool is_executing_destructor_;
};