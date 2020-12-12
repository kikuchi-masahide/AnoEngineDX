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
	//���_���C�A�E�g�̏��t�H�[�}�b�g
	enum class VertexLayoutFormat {
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		size
	};
	//���_���C�A�E�g��input_classificastion
	enum class VertexLayoutInputClassification {
		INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		INPUT_CLASSIFICATION_PER_INSTANCE_DATA,
		size
	};
	//�v���~�e�B�u�g�|���W
	enum class PrimitiveTopologyType {
		POINT,
		LINE,
		TRIANGLE,
		size
	};
}
