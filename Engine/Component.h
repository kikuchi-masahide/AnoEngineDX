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
	//�������w��shared_ptr��ݒ肷��(This�̍ۂɗp����)
	void SetSelfSharedptr(std::shared_ptr<Component> comp);
	const int upd_priority_;
	GameObjectHandle const obj_;
	//HACK:GameObject��scene_���ڂ��΁A�������팸�ɂ͂Ȃ�B����Ӗ��͂��邩?����sizeof(Component)=48
	Scene* const scene_;
	//���g���w���n���h����Ԃ��֐�
	template<class T>
	ComponentHandle<T> This()
	{
#ifdef _DEBUG
		//�f�o�b�O����dynamic_pointer_cast���s�����S�Ȍ^�ϊ����m�F����
		if (std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(self_.lock())) {
			return ComponentHandle<T>(result);
		}
		else {
			Log::OutputCritical("Component::This() for bad type of ComponentHandle");
			return ComponentHandle<T>();
		}
#else
		return ComponentHandle<T>(std::static_pointer_cast<T>(self_.lock()));
#endif
	}
protected:
private:
	//&�ŃC���X�^���X�̃|�C���^���擾�����Ȃ�
	Component* operator&() const noexcept;
	std::weak_ptr<Component> self_;
	bool delete_flag_;
};