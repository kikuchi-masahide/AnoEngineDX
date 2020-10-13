#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace Microsoft::WRL;

/// <summary>
/// �X���b�v�`�F�[���̃}�l�[�W��
/// </summary>
class SwapChainManager : public boost::noncopyable{
private:
	std::vector<ComPtr<IDXGISwapChain1>> mSwapChains;
	//�X���b�v�`�F�[���̐ݒ�̂������ʂ������
	static DXGI_SWAP_CHAIN_DESC1 mBaseDesc;
public:
	/// <summary>
	/// �X���b�v�`�F�[����������
	/// </summary>
	/// <returns>�L�[</returns>
	unsigned int AddSwapChain(
		ComPtr<IDXGIFactory6>& _factory, ComPtr<ID3D12CommandQueue>& _cmdqueue,
		HWND _hwnd, UINT _width, UINT _height);
};