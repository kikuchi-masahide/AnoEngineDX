#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

class DX12Factory;
class DX12CmdQueue;
class DX12Device;
class DX12DescriptorHeap;
class DX12CmdList;

/// <summary>
/// �X���b�v�`�F�[���̃}�l�[�W��
/// </summary>
class SwapChainManager final : public boost::noncopyable {
private:
	std::vector<ComPtr<IDXGISwapChain4>> mSwapChains;
	//�e�X���b�v�`�F�[���ɑ΂���r���[�����f�B�X�N���v�^�q�[�v
	//(1�̃f�B�X�N���v�^�q�[�v��1�̃X���b�v�`�F�[�����Ǘ�����Ɗm�񂷂�)
	std::vector<boost::shared_ptr<DX12DescriptorHeap>> mDescHeaps;
	//���\�[�X(mBackBuffers[BackBuffer�����ڂ�][id])
	std::vector<ComPtr<ID3D12Resource>> mBackBuffers[2];
	//�X���b�v�`�F�[���̐ݒ�̂������ʂ������
	static DXGI_SWAP_CHAIN_DESC1 mBaseDesc;
	//���\�[�X�o���A�̐ݒ���s���\����
	static D3D12_RESOURCE_BARRIER mResourceBarrierDesc;
public:
	void Initialize();
	void CleanUp();
	/// <summary>
	/// �X���b�v�`�F�[����������
	/// </summary>
	/// <returns>�L�[</returns>
	unsigned int AddSwapChain(
		DX12Factory* _factory, DX12CmdQueue* _cmdqueue, DX12Device* _device, HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap);
	/// <summary>
	/// �S�X���b�v�`�F�[�����t���b�v
	/// </summary>
	void FlipAll();
	/// <summary>
	/// _id�̎w���X���b�v�`�F�[���������_�[�^�[�Q�b�g�Ɏw�肵���\�[�X�o���A��ݒ�D�w�i�F�������ɐݒ肷��
	/// </summary>
	void SetAndClearRenderTarget(unsigned int _id,DX12CmdList* _list,float _r,float _g,float _b);
};