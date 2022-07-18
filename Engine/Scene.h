//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObject.h"
#include "Component.h"
#include "InputSystem.h"
#include "Math/Math.h"

class Game;
class UIScreen;
class InputSystem;

//HACK:�v���t�@�C�����ł����玎������
//�E�R���|�[�l���g�̏�������std::any�ł��Ȃ�ł��g���āAAdd~Component�ōs��Ȃ��悤�ɂ���
//�EComponentHandle��std::weak_ptr���g���Ȃ���?�g�����ꍇ�̑��x��r
//�Estd::set�Ȃǂ�std::vector���ʂ̃R���e�i�ɕς����ꍇ�̔�r
//�Epool���g����Ƃ���ł����Ǝg�����ꍇ�̔�r

/// <summary>
/// �V�[����\���N���X
/// </summary>
class Scene {
public:
	/// <summary>
	/// �v�[���Ȃǂ̏������BGame�̃R���X�g���N�^����Ăяo��
	/// </summary>
	static void InitMemory();
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
	GameObjectHandle AddObject(int comphandle_reserve_num = 4);
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	/// <summary>
	/// obj�̎w��GameObject��UpdateComponent��ǉ�
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(GameObjectHandle obj, Args... args) {
		//HACK:Scene�̃f�X�g���N�^���s���Apanding_~_comps_�������ɑ����Ă������Ƃ�h�~
		//�R���X�g���N�^���̂̎��s��Add~Component�ōs��Ȃ���΂���v��Ȃ��񂾂���......
		if (is_executing_destructor_) {
			return ComponentHandle<T>();
		}
		auto itr = id_objpointer_map_.find(obj);
		if (itr == id_objpointer_map_.end()) {
			Log::OutputCritical("AddUpdateComponent to unexisting GameObject");
			return ComponentHandle<T>();
		}
		T* comp_p = DBG_NEW T(this, obj, args...);
		panding_update_components_.push_back(comp_p);
		itr->second->AddComponent(comp_p->This<Component>());
		return comp_p->This<T>();
	}
	/// <summary>
	/// obj�̎w��GameObject��OutputComponent��ǉ�
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(GameObjectHandle obj, Args... args) {
		if (is_executing_destructor_) {
			return ComponentHandle<T>();
		}
		auto itr = id_objpointer_map_.find(obj);
		if (itr == id_objpointer_map_.end()) {
			Log::OutputCritical("AddOutputComponent to unexisting GameObject");
			return;
		}
		T* comp_p = DBG_NEW T(this, obj, args...);
		panding_output_components_.push_back(comp_p);
		itr->second->AddComponent(comp_p->This<Component>());
		return comp_p->This<T>();
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

	void Erase(GameObjectHandle handle);
	int GetGameObjectNumber();
	int GetUpdateComponentNumber();
	int GetOutputComponentNumber();

protected:
	virtual ~Scene();
private:
	//�����̎��S�X�V�E�o�̓R���|�[�l���g��Update���Ăяo��(�ۗ��R���|�[�l���g�̂���͎��s���Ȃ�)
	void LaunchUpdateComponents();
	void LaunchOutputComponents();
	void DeleteUIScreen();
	//UIScreen��Update��������Ăяo��
	void LaunchUIScreenUpdate();
	//UIScreen��Output��������Ăяo��
	void LaunchOutputUIScreens();
	//���̃I�u�W�F�N�g�̃|�C���^��delete���f�X�g���N�^���Ă�
	void DeleteObject(GameObject* _object);
	GameObject* operator&() const noexcept;
	bool delete_flag_;
	friend Game;
	bool delete_check_;
	//���g�̎���GameObject�̃��X�g�y�ѕۗ����̃I�u�W�F�N�g
	std::vector<GameObject*> objs_;
	//���g�̎��X�V�E�o�̓R���|�[�l���g�̃��X�g�C����ѕۗ��R���|�[�l���g
	//HACK:�]�T��������ʂ̃R���e�i�ɕς����ꍇ�̃p�t�H�[�}���X��r
	std::vector<Component*> update_components_;
	std::vector<Component*> panding_update_components_;
	std::vector<Component*> output_components_;
	std::vector<Component*> panding_output_components_;
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

	constexpr static int kMaxObjNum_ = 10000;
	//GameObject��ۑ����郁�����v�[��
	static std::optional<boost::pool<>> obj_pool_;
	//HACK:�A���P�[�^�ڂ��ăv�[���g��?
	static std::map<GameObjectHandle, GameObject*> id_objpointer_map_;
	//����GameObject��ǉ�����Ƃ��g��id
	int next_obj_id_;
	//�������ׂ��I�u�W�F�N�g��id
	std::vector<GameObjectHandle> erase_objs_;
	void ProcessPandingComps();
	void ProcessPandingUIScreens();
};