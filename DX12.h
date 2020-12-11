#pragma once

#include "DX12ConfigEnums.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")

class DX12Factory;
class DX12Device;
class DX12CmdAllocator;
class DX12CmdList;
class DX12CmdQueue;
class SwapChainManager;
class DX12DescriptorHeap;
class DX12Resource;

/// <summary>
/// Game�Ɋ܂܂��CDirectX12�����b�v�����N���X
/// </summary>
class DX12 :public boost::noncopyable
{
private:
	DX12Factory* mFactory;
	DX12Device* mDevice;
	DX12CmdAllocator* mCmdAllocator;
	DX12CmdList* mCmdList;
	DX12CmdQueue* mCmdQueue;
	SwapChainManager* mSwapChainManager;
	void EnableDebugLayer();
	//ProcessCommands�Ɋ܂܂��t�F���X�ɂ��҂����Ԃ̕���
	void FenceWaitingInProcessCommands();
public:
	DX12();
	void Initialize();
	void CleanUp();
	/// <summary>
	/// �X���b�v�`�F�[�������
	/// </summary>
	/// <param name="_descheap">�X���b�v�`�F�[���ƕR�Â���f�B�X�N���v�^�q�[�v</param>
	/// <returns>�X���b�v�`�F�[���̃L�[</returns>
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap);
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�����
	/// </summary>
	/// <param name="_type">�f�B�X�N���v�^�q�[�v�̎��</param>
	/// <param name="_vis">�V�F�[�_��������ۂ�</param>
	/// <param name="_num">���L�f�B�X�N���v�^�̐�</param>
	/// <returns>�f�B�X�N���v�^�q�[�v��\��shared_ptr</returns>
	boost::shared_ptr<DX12DescriptorHeap> CreateDescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num);
	/// <summary>
	/// �e�t���[���ŕK�v�ȃR�}���h��ǉ������������ƌĂяo��
	/// </summary>
	void ProcessCommands();
	/// <summary>
	/// _id�̎w���X���b�v�`�F�[���������_�[�^�[�Q�b�g�Ɏw�肵�w�i�F�h��Ԃ�
	/// </summary>
	void SetAndClearRenderTarget(unsigned int _id,float _r,float _g,float _b);
	//���_�o�b�t�@�쐬
	boost::shared_ptr<DX12Resource> CreateVertexBuffer(UINT64 _width);
	//���\�[�X�̃}�b�v
	void* Map(boost::shared_ptr<DX12Resource> _resource);
	void Unmap(boost::shared_ptr<DX12Resource> _resource);
};