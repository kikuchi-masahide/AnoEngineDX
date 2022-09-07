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
/// 1つのウィンドウを表すクラス(コピー不可)
/// </summary>
class Window : public boost::noncopyable {
public:
	/// <summary>
	/// ウィンドウの作成を同時に行うコンストラクタ
	/// </summary>
	/// <param name="_wndproc">ウィンドウプロシャ―ジャ</param>
	/// <param name="_classID">ウィンドウクラスを表すID</param>
	/// <param name="_width">幅</param>
	/// <param name="_height">高さ</param>
	/// <param name="_windowTitle">ウィンドウのタイトル</param>
	Window(WNDPROC wndproc, LPCWSTR classid, int width, int height, LPCWSTR window_title);
	~Window();
	/// <summary>
	/// 与えられたcommand queueを用いてswapchainを作成する
	/// </summary>
	void CreateSwapChain(DX12::Master& dx12, std::shared_ptr<DX12::CommandQueue> cmd_queue);
	/// <summary>
	/// このswapchainをrender targetにするコマンドを、cmd_listにセットする。
	/// </summary>
	void SetRenderTarget(std::shared_ptr<DX12::GraphicsCommandList> cmd_list) const;
	/// <summary>
	/// このswapchainをrender targetにするコマンドを、depth stencil bufferを指定してcmd_listにセットする
	/// </summary>
	/// <param name="dsv_heap">depth buffer viewのあるdescriptor heap</param>
	/// <param name="dsv_index">dsv_heapの何番目にviewがあるか</param>
	void SetRenderTarget(std::shared_ptr<DX12::GraphicsCommandList> cmd_list,
		std::shared_ptr<DX12::DescriptorHeap> dsv_heap, int dsv_index) const;
	/// <summary>
	/// このswapchainのbackbufferのstateを、beforeからafterへ変更する
	/// </summary>
	void SetBackbufferStateBarrier(std::shared_ptr<DX12::GraphicsCommandList> cmd_list,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const;
	/// <summary>
	/// このswapchainのbackbufferを、指定色でクリアする
	/// </summary>
	void ClearBackbuffer(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, float r, float g, float b) const;
	/// <summary>
	/// このwindowのswapchainをflipする
	/// </summary>
	void Flip() const;
	HWND GetWindowHandle() const;
	RECT GetWindowSize() const;
	std::weak_ptr<DX12::SwapChain> GetSwapchain() const;
private:
	void Create(WNDPROC wndproc, LPCWSTR classid, int width, int height, LPCWSTR window_title);
	WNDCLASSEX window_class_;
	HWND hwnd_;
	//ウィンドウに付属するswapchain(使わないならばnullptr)
	std::shared_ptr<DX12::SwapChain> swapchain_;
};