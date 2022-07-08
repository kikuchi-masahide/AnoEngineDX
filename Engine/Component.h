//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Scene;
class Game;

#include "ComponentHandle.h"
#include "GameObjectHandle.h"

/// <summary>
/// �I�u�W�F�N�g�̋@�\��\���N���X
/// �I�u�W�F�N�g�̍X�V������R���|�[�l���g�Əo�͂�����R���|�[�l���g�͗������̊��N���X����h��������
/// </summary>
// HACK:�R���|�[�l���g�����̃N���X�̔h���ɂ����C�����̒��g�E���f�[�^���^�N���X�ɂ���̂͂��肩?
class Component {
public:
	/// <param name="_order">�D��x �����قǐ�ɌĂяo�����</param>
	Component(GameObjectHandle handle, int order = 0);
	/// <summary>
	/// �R���|�|�l���g�̍X�V����
	/// </summary>
	virtual void Update() = 0;
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	const int upd_priority_;
	const GameObjectHandle obj_;
protected:
	virtual ~Component();
	//���g���w���n���h����Ԃ��֐�
	template<class T>
	ComponentHandle<T> This()
	{
		return ComponentHandle<T>((T*)this, &handles_);
	}
private:
	friend class GameObject;
	//&�ŃC���X�^���X�̃|�C���^���擾�����Ȃ�
	Component* operator&() const noexcept;
	//�������w���n���h���̏W���̃|�C���^(void*���g���͉̂��Ƃ�������߂���)
	std::unordered_set<void*> handles_;
	bool delete_check_;
	bool delete_flag_;
};