//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"
#include "DescriptorRange.h"

namespace DX12 {
	//HACK:�T���v���[���ςɂł���悤�ɂ���ׂ���?
	/// <summary>
	/// ���[�g�V�O�l�`���̃N���X
	/// </summary>
	class RootSignature final:public boost::noncopyable
	{
	public:
		/// <param name="root_param_num">RootParameter�̐�(std::vector�̏���reserve��)</param>
		RootSignature(int root_param_num);
		~RootSignature();
		//���݂̓��e�ŃV���A���C�Y����(�V���A���C�Y�ニ�[�g�p�����^��T���v���[�͕ύX�ł��Ȃ�)
		void Serialize(ComPtr<ID3D12Device> device);
		void SetRootSignature(ComPtr<ID3D12GraphicsCommandList> list);
		//GraphicsPipeline�p
		ID3D12RootSignature* GetRawPtr();
		//DescriptorTable�^�C�v��RootParameter��ǉ�����
		void AddRootParameterAsDescriptorTable(std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis);
	private:
		ComPtr<ID3D12RootSignature> root_signature_;
		//�V���A���C�Y�ςȂ��true
		bool serialized_;
		std::vector<CD3DX12_ROOT_PARAMETER> root_params_;
		//root_params_�̐ݒ�Ŏg��DescriptorRange�̔z��̔z��
		//���delete����
		std::vector<D3D12_DESCRIPTOR_RANGE*> ranges_;
	};
}

