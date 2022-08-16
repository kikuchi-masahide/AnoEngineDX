//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	enum class ResourceBarrierState :int {
		PRESENT = 0,
		RENDER_TARGET = 0x4,
		COPY_DEST = 0x400,
		PIXEL_SHADER_RESOURCE = 0x80,
		UPLOAD_GENERIC_READ = 2755
	};
	//�V�F�[�_�̃^�C�v
	enum class ShaderType :unsigned char {
		VERTEX,
		PIXEL,
		size
	};
	//���_���C�A�E�g�̏��t�H�[�}�b�g
	enum class VertexLayoutFormat :unsigned char {
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		R32G32_FLOAT,
		R32_UINT,
		size
	};
	//GraphicsPipeline�Ŏw�肷��v���~�e�B�u�g�|���W(�\���v�f�̎w��)
	enum class PrimitiveTopology :unsigned char {
		POINT,
		LINELIST,//�Ɨ����������̏W��
		LINESTRIP,//�O�̒��_�ƂȂ����
		TRIANGLELIST,//�Ɨ������O�p�`�̏W��
		TRIANGLESTRIP,//�O�̎O�p�`�ƂȂ������O�p�`�̏W����
		size
	};
	//���\�[�X�̃q�[�v��CPU����̃A�N�Z�X�̉s�̐ݒ�
	enum class ResourceHeapType :int {
		DEFAULT = 1,//CPU����A�N�Z�X�ł��Ȃ�
		UPLOAD = 2,//CPU����A�N�Z�X�ł���
		READBACK = 3,//CPU����ǂݎ���
	};
	//�f�B�X�N���v�^�q�[�v�̎��
	enum class DescriptorHeapType :int {
		CBV_SRV_UAV = 0,
		RTV = 2,
		DSV = 3
	};
	//�V�F�[�_����̉��͈͂Ɋւ���萔
	enum class DescriptorHeapShaderVisibility :unsigned char {
		NONE = 0,
		SHADER_VISIBLE = 1
	};
	//�f�B�X�N���v�^�����W�̃^�C�v
	enum class DescriptorRangeType :unsigned char {
		SRV = 0,
		UAV = 1,
		CBV = 2
	};
	//���[�g�p�����[�^�̃V�F�[�_�[����̌����
	enum class RootParameterShaderVisibility :unsigned char {
		ALL = 0,
		VERTEX = 1,
		PIXEL = 5
	};
}
