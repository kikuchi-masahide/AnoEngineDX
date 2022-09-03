//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include <Windows.h>

namespace DX12 {
	class Master;
	class SwapChain;
	class CommandQueue;
	class GraphicsCommandList;
	class DescriptorHeap;
}

/// <summary>
/// 1�̃E�B���h�E��\���N���X(�R�s�[�s��)
/// </summary>
class Window : public boost::noncopyable {
public:
	/// <summary>
	/// �E�B���h�E�̍쐬�𓯎��ɍs���R���X�g���N�^
	/// </summary>
	/// <param name="_wndproc">�E�B���h�E�v���V���\�W��</param>
	/// <param name="_classID">�E�B���h�E�N���X��\��ID</param>
	/// <param name="_width">��</param>
	/// <param name="_height">����</param>
	/// <param name="_windowTitle">�E�B���h�E�̃^�C�g��</param>
	Window(WNDPROC wndproc, LPCWSTR classid, int width, int height, LPCWSTR window_title);
	~Window();
	/// <summary>
	/// �^����ꂽcommand queue��p����swapchain���쐬����
	/// </summary>
	void CreateSwapChain(DX12::Master& dx12, std::shared_ptr<DX12::CommandQueue> cmd_queue);
	/// <summary>
	/// ����swapchain��render target�ɂ���R�}���h���Acmd_list�ɃZ�b�g����B
	/// </summary>
	void SetRenderTarget(std::shared_ptr<DX12::GraphicsCommandList> cmd_list) const;
	/// <summary>
	/// ����swapchain��render target�ɂ���R�}���h���Adepth stencil buffer���w�肵��cmd_list�ɃZ�b�g����
	/// </summary>
	/// <param name="dsv_heap">depth buffer view�̂���descriptor heap</param>
	/// <param name="dsv_index">dsv_heap�̉��Ԗڂ�view�����邩</param>
	void SetRenderTarget(std::shared_ptr<DX12::GraphicsCommandList> cmd_list,
		std::shared_ptr<DX12::DescriptorHeap> dsv_heap, int dsv_index) const;
	/// <summary>
	/// ����swapchain��backbuffer��state���Abefore����after�֕ύX����
	/// </summary>
	void SetBackbufferStateBarrier(std::shared_ptr<DX12::GraphicsCommandList> cmd_list,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const;
	/// <summary>
	/// ����swapchain��backbuffer���A�w��F�ŃN���A����
	/// </summary>
	void ClearBackbuffer(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, float r, float g, float b) const;
	/// <summary>
	/// ����window��swapchain��flip����
	/// </summary>
	void Flip() const;
	HWND GetWindowHandle() const;
	RECT GetWindowSize() const;
	std::weak_ptr<DX12::SwapChain> GetSwapchain() const;
private:
	void Create(WNDPROC wndproc, LPCWSTR classid, int width, int height, LPCWSTR window_title);
	WNDCLASSEX window_class_;
	HWND hwnd_;
	//�E�B���h�E�ɕt������swapchain(�g��Ȃ��Ȃ��nullptr)
	std::shared_ptr<DX12::SwapChain> swapchain_;
};