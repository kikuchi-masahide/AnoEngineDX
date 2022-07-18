//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "GameObject.h"
#include "ComponentHandle.h"

class Scene;

/// <summary>
/// �I�u�W�F�N�g�̋@�\��\���N���X
/// �I�u�W�F�N�g�̍X�V������R���|�[�l���g�Əo�͂�����R���|�[�l���g�͗������̊��N���X����h��������
/// </summary>
class Component {
public:
	/// <param name="_order">�D��x �����قǐ�ɌĂяo�����</param>
	Component(Scene* scene, GameObjectHandle handle, int order = 0);
	virtual void Initialize();
	/// <summary>
	/// Scene�̃f�X�g���N�^�����s���ꂽ�ۂ̌Ăяo���Ȃǂł́AAdd~Component���Ă��K���L����Component���Ԃ��Ă���Ƃ͌���Ȃ����Ƃɒ���
	/// </summary>
	virtual ~Component();
	/// <summary>
	/// �R���|�|�l���g�̍X�V����
	/// </summary>
	virtual void Update() = 0;
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	const int upd_priority_;
	GameObjectHandle const obj_;
	Scene* const scene_;
	//���g���w���n���h����Ԃ��֐�
	template<class T>
	ComponentHandle<T> This()
	{
		return ComponentHandle<T>((T*)this, &handles_);
	}
protected:
private:
	//&�ŃC���X�^���X�̃|�C���^���擾�����Ȃ�
	Component* operator&() const noexcept;
	//�������w���n���h���̏W���̃|�C���^(void*���g���͉̂��Ƃ�������߂���)
	std::unordered_set<void*> handles_;
	bool delete_check_;
	bool delete_flag_;
};