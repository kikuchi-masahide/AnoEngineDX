//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "ComponentHandle.h"
#include "GameObjectHandle.h"

class Scene;

/// <summary>
/// �I�u�W�F�N�g�̋@�\��\���N���X
/// �I�u�W�F�N�g�̍X�V������R���|�[�l���g�Əo�͂�����R���|�[�l���g�͗������̊��N���X����h��������
/// </summary>
class Component {
public:
	const int upd_priority_;
	GameObjectHandle const obj_;
	/// <param name="_order">�D��x �����قǐ�ɌĂяo�����</param>
	Component(GameObjectHandle handle, int order = 0);
	/// <summary>
	/// �R���X�g���N�^�̎��s��A�ʃX���b�h�ŌĂяo����鏉�����֐�
	/// (�I�[�o�[���C�h��Ŋ�{�N���X��AsyncInitialize()���Ăяo������)
	/// </summary>
	virtual void AsyncInitialize();
	/// <summary>
	/// Scene�̃f�X�g���N�^�����s���ꂽ�ۂ̌Ăяo���Ȃǂł́AAdd~Component���Ă��K���L����Component���Ԃ��Ă���Ƃ͌���Ȃ����Ƃɒ���
	/// </summary>
	virtual ~Component();
	/// <summary>
	/// �R���|�|�l���g�̍X�V����
	/// </summary>
	virtual void Update() = 0;
	//���g���w���n���h����Ԃ��֐�
	template<class T>
	ComponentHandle<T> This()
	{
#ifdef _DEBUG
		//�f�o�b�O����dynamic_pointer_cast���s�����S�Ȍ^�ϊ����m�F����
		if (std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(this_sh_)) {
			return ComponentHandle<T>(result);
		}
		else {
			Log::OutputCritical("Component::This() for bad type of ComponentHandle");
			return ComponentHandle<T>();
		}
#else
		return ComponentHandle<T>(std::static_pointer_cast<T>(this_sh_));
#endif
	}
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	//this_sh_���Z�b�g����(Scene::AddComponent�����x�����Ăяo�����)
	void SetSharedPtr(std::shared_ptr<Component> comp);
	//this_sh_�����Z�b�g���A���̊֐��̌Ăяo������Ƀf�X�g���N�^���s(�����͌Ăяo�����`�F�b�N�p)
	void ResetSharedPtr(std::shared_ptr<Component> comp);
protected:
private:
	//&�ŃC���X�^���X�̃|�C���^���擾�����Ȃ�
	Component* operator&() const noexcept;
	std::shared_ptr<Component> this_sh_;
	bool delete_flag_;
};