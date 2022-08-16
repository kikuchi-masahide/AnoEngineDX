//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObjectHandle.h"
#include "ElementContainer.h"
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
	GameObjectHandle AddObject();
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	/// <summary>
	/// obj�̎w��GameObject��UpdateComponent��ǉ�(GameObject::AddUpdateComponent����Ăяo�����)
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(std::shared_ptr<GameObject> obj, Args... args);
	/// <summary>
	/// obj�̎w��GameObject��OutputComponent��ǉ�(GameObject::AddOutputComponent����Ăяo�����)
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(std::shared_ptr<GameObject> obj, Args... args);
	/// <summary>
	/// ����upd_prio�����R���|�[�l���g�����s����O�Ɏ��s����֐���o�^����
	/// (�R���|�[�l���g�����݂��Ȃ���Ύ��s���Ȃ�)
	/// </summary>
	void SetOutputCompsPreFunc(int upd_prio, std::function<void()> func);
	/// <summary>
	/// ����upd_prio�����Ō�̃R���|�[�l���g�����s������Ɏ��s����֐���o�^����
	/// (�R���|�[�l���g�����݂��Ȃ���Ύ��s���Ȃ�)
	/// </summary>
	void SetOutputCompsPostFunc(int upd_prio, std::function<void()> func);
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

	//����GameObject���A�t���[���؂�ւ��O�ɏ�������(GameObject::SetDeleteFlag����Ăяo�����)
	void Erase(std::weak_ptr<GameObject> ptr);
	void Erase(std::weak_ptr<Component> ptr);
	int GetGameObjectNumber();
	int GetUpdateComponentNumber();
	int GetOutputComponentNumber();
protected:
	virtual ~Scene();
private:
	//�����̎��S�X�V�E�o�̓R���|�[�l���g��Update���Ăяo��(�ۗ��R���|�[�l���g�̂���͎��s���Ȃ�)
	void DeleteUIScreen();
	//UIScreen��Update��������Ăяo��
	void LaunchUIScreenUpdate();
	//UIScreen��Output��������Ăяo��
	void LaunchOutputUIScreens();
	GameObject* operator&() const noexcept;
	bool delete_flag_;
	friend Game;
	bool delete_check_;
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
	void ProcessPandingComps();
	void ProcessPandingUIScreens();
	ElementContainer element_container_;
};

template<class T, class ...Args>
inline ComponentHandle<T> Scene::AddUpdateComponent(std::shared_ptr<GameObject> obj, Args ...args)
{
	if (is_executing_destructor_) {
		return ComponentHandle<T>();
	}
	return element_container_.AddUpdateComponent<T>(obj, args...);
}

template<class T, class ...Args>
inline ComponentHandle<T> Scene::AddOutputComponent(std::shared_ptr<GameObject> obj, Args ...args)
{
	if (is_executing_destructor_) {
		return ComponentHandle<T>();
	}
	return element_container_.AddOutputComponent<T>(obj, args...);
}

