//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "Math/Math.h"

/// <summary>
/// �e�L�[�E�}�E�X�{�^���̏�Ԃ�\��enum
/// None:off->off
/// Pressed:off->on
/// Released:on->off
/// Held:on->on
/// </summary>
enum class ButtonState : char {
	None = 0,
	Pressed,
	Released,
	Held
};

/// <summary>
/// Game�����C���͂��Ǘ�����N���X
/// </summary>
class InputSystem : public boost::noncopyable {
public:
	/// <summary>
	/// Game����̂݌Ă΂ꂤ��R���X�g���N�^
	/// </summary>
	InputSystem();
	/// <summary>
	/// �V�[���C�I�u�W�F�N�g����Update���Ăяo�����O�̑O����(Game�݂̂��Ăяo����)
	void ProcessBeforeUpdate();
	/// <summary>
	/// �w��L�[�̏�Ԃ�ButtonState�^�ŕԂ�
	/// </summary>
	/// <param name="_key">�����L�[�̏ꍇ�Ή������啶���̃L�[�R�[�h('A'�Ȃ�)�C����ȊO�̏ꍇVK_�`�`�̊Y������ϐ�</param>
	/// <returns></returns>
	ButtonState GetKeyState(int _key) const;
	/// <summary>
	/// �O�t���[�����猻�t���[���ł̃}�E�X�̈ړ����x�N�g���Ŏ擾(���ʒu-�O�ʒu�C�������_)
	/// </summary>
	MatVec::Vector2 GetMouseMove() const;
	/// <summary>
	/// �}�E�X�̃X�N���[�����W���擾(���㌴�_�͂��̂܂�)
	/// </summary>
	MatVec::Vector2 GetMouseScreenPos() const;
private:
	//�O�E���t���[���̃L�[�{�[�h�̏�
	BYTE key_state_[2][256];
	//mKeyState�CmMousePos�ł̌��t���[���󋵂̓Y����
	unsigned int cur_index_;
	//�O�E���t���[���̃}�E�X�J�[�\���ʒu(����������̓X�N���[�����W�ł��邱�Ƃɒ���)
	MatVec::Vector2 mouse_pos_[2];
};