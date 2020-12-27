#pragma once

#include "DX12CommonInclude.h"

#include "DX12ConfigEnums.h"

class DX12Device;
class DX12Resource;

class DX12DescriptorHeap final :public boost::noncopyable {
public:
	//�R���X�g���N�^
	DX12DescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num, DX12Device* _device);
	~DX12DescriptorHeap();
	/// <summary>
	/// _n�Ԗڂ�CPU�n���h���擾
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(unsigned int _n);
private:
	//���L�f�B�X�N���v�^�̐�
	int mNum;
	DX12Config::DescriptorHeapType mType;
	DX12Config::ShaderVisibility mShaderVisibility;
	ComPtr<ID3D12DescriptorHeap> mDescHeap;
	//1�f�B�X�N���v�^������̃f�B�X�N���v�^�n���h���n���h��
	SIZE_T mDescriptorHandleSize;
	//�f�B�X�N���v�^�q�[�v�������ŗp����\����(���ʂ���ݒ�͂���ɐݒ肵�Ă���)
	static D3D12_DESCRIPTOR_HEAP_DESC mHeapDescStr;
	//DX12Config::DescriptorHeapType�̊e�萔�����������ǂ̒萔�ɑΉ����邩
	static D3D12_DESCRIPTOR_HEAP_TYPE mDescriptorHeapTypeCorrespond[(unsigned char)DX12Config::DescriptorHeapType::size];
	//DX12Config::ShaderVisibility�̊e�萔�����������ǂ̒萔�ɑΉ����邩
	static D3D12_DESCRIPTOR_HEAP_FLAGS mShaderVisibilityCorrespond[(unsigned char)DX12Config::ShaderVisibility::size];
};