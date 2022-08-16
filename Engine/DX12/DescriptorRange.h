//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "ConfigEnums.h"

namespace DX12 {
	/// <summary>
	/// DescriptorTable�^�C�v��RootParameter�Ɋ܂܂��DescriptorRange
	/// </summary>
	struct DescriptorRange {
	public:
		//�ߑ�����descriptor�̎��
		int num_descriptors_;
		//�ߑ�����descriptor�̎��
		DescriptorRangeType range_type_;
		//�V�F�[�_���W�X�^�̉��Ԗڂ����L���邩
		//(�Ⴆ��mBaseShaderRegister = 2,mNumDescriptors = 4,mType = CBV�Ȃ�΁C
		//���̃f�B�X�N���v�^�����W��b2����b5���L����)
		int base_shader_register_;
		//�f�B�X�N���v�^�q�[�v�̉��Ԗڂ����L���邩
		//(�Ⴆ��mBaseHeapRegister = 2,mNumDescriptors = 4�Ȃ�΁C
		//�f�B�X�N���v�^�q�[�v��2����5�Ԗڂ��L����)
		//-1�Ȃ�΁AD3D12_DESCRIPTOR_RANGE_OFFSET_APPEND�A�܂�O��Range�̒���Ɖ��߂���
		int base_heap_register_;
		DescriptorRange(int num_desc, DescriptorRangeType range_type, int base_shader_register,
			int base_heap_register = -1)
			:num_descriptors_(num_desc),range_type_(range_type),base_shader_register_(base_shader_register),
			base_heap_register_(base_heap_register)
		{}
	};
}