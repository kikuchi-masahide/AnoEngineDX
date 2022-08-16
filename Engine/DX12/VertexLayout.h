//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "ConfigEnums.h"

namespace DX12 {
	/// <summary>
	/// ���_���C�A�E�g�̈�̗v�f��\�� �p�C�v���C���ɂ͂��̔z��𒸓_���C�A�E�g�S�̂Ƃ��ēn��
	/// </summary>
	struct VertexLayoutUnit {
	public:
		//�Z�}���e�B�b�N��("POSITION"�Ȃ�)
		LPCSTR semantic_name_;
		//�t�H�[�}�b�g
		VertexLayoutFormat format_;
		//���̓X���b�g�C���f�b�N�X
		UINT input_slot_;
		VertexLayoutUnit(LPCSTR sem, VertexLayoutFormat format, UINT slot)
			:semantic_name_(sem), format_(format), input_slot_(slot)
		{}
	};
}
