#pragma once
/// <summary>
/// �N���XDX12�����C���^�[�t�F�[�X�ŗp����enum
/// </summary>

namespace DX12Config {
	//�f�B�X�N���v�^�q�[�v�̎��
	enum class DescriptorHeapType :unsigned char {
		RTV,
		CBV,
		SRV,
		UAV,
		DSV,
		size
	};
	//�V�F�[�_����̉��͈͂Ɋւ���萔
	enum class ShaderVisibility :unsigned char {
		NONE,
		size
	};
	//���\�[�X�̃q�[�v��CPU����̃A�N�Z�X�̉s�̐ݒ�
	enum class ResourceHeapType {
		DEFAULT,//CPU����A�N�Z�X�ł��Ȃ�
		UPLOAD,//CPU����A�N�Z�X�ł���
		READBACK,//CPU����ǂݎ���
		size
	};
	//�V�F�[�_�̃^�C�v
	enum class ShaderType {
		VERTEX,
		PIXEL,
		size
	};
}
