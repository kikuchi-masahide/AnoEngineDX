//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "window.h"
#include "DX12/Master.h"
#include "DX12/CommandQueue.h"
#include "DX12/SwapChain.h"
#include "DX12/GraphicsCommandList.h"
#include "DX12/DescriptorHeap.h"
#include "DX12/Texture2D.h"

Window::Window(WNDPROC wndproc, LPCWSTR classid, int width, int height, LPCWSTR window_title)
{
	Create(wndproc, classid, width, height, window_title);
	::ShowWindow(hwnd_, SW_SHOW);
}

Window::~Window()
{
	UnregisterClass(window_class_.lpszClassName, window_class_.hInstance);
}

void Window::CreateSwapChain(DX12::Master& dx12, DX12::CommandQueue cmd_queue)
{
	assert(!swapchain_.IsValid());
	auto rect = GetWindowSize();
	swapchain_ = dx12.CreateSwapChain(cmd_queue, hwnd_, rect.right - rect.left, rect.bottom - rect.top);
}

void Window::SetRenderTarget(DX12::GraphicsCommandList cmd_list) const
{
	assert(swapchain_.IsValid());
	auto bbindex = swapchain_.GetCurrentBackBufferIndex();
	auto rtv_desc = swapchain_.GetDescriptorHeap();
	cmd_list.SetRenderTargets(rtv_desc, bbindex, 1);
}

void Window::SetRenderTarget(DX12::GraphicsCommandList cmd_list,
	DX12::DescriptorHeap dsv_heap, int dsv_index) const
{
	assert(swapchain_.IsValid());
	auto bbindex = swapchain_.GetCurrentBackBufferIndex();
	auto rtv_desc = swapchain_.GetDescriptorHeap();
	cmd_list.SetRenderTargets(rtv_desc, bbindex, 1, dsv_heap,dsv_index);
}

void Window::SetBackbufferStateBarrier(DX12::GraphicsCommandList cmd_list,
	D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const
{
	assert(swapchain_.IsValid());
	auto backbuffer = swapchain_.GetCurrentBackBuffer();
	cmd_list.SetResourceBarrier(DX12::ResourceBarrierUnit(backbuffer, before, after));
}

void Window::ClearBackbuffer(DX12::GraphicsCommandList cmd_list, float r, float g, float b) const
{
	assert(swapchain_.IsValid());
	auto bbindex = swapchain_.GetCurrentBackBufferIndex();
	auto rtv_desc = swapchain_.GetDescriptorHeap();
	cmd_list.ClearRenderTargetView(rtv_desc, bbindex, r, g, b);
}

void Window::Flip()
{
	if (swapchain_.IsValid()) {
		swapchain_.Flip();
	}
}

HWND Window::GetWindowHandle() const
{
	return hwnd_;
}

RECT Window::GetWindowSize() const
{
	RECT rect;
	GetClientRect(hwnd_, &rect);
	return rect;
}

DX12::SwapChain Window::GetSwapchain() const
{
	return swapchain_;
}

void Window::Create(WNDPROC wndproc, LPCWSTR classid, int width, int height, LPCWSTR window_title)
{
	window_class_ = {};
	RECT wrc = {};
	wrc.left = 0;
	wrc.top = 0;
	wrc.right = width;
	wrc.bottom = height;
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	window_class_.hInstance = GetModuleHandle(nullptr);
	window_class_.cbSize = sizeof(WNDCLASSEX);
	window_class_.lpfnWndProc = (WNDPROC)wndproc;
	window_class_.lpszClassName = classid;
	window_class_.hCursor = (HCURSOR)LoadImage(
		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED
	);
	RegisterClassEx(&window_class_);
	hwnd_ = CreateWindow(
		window_class_.lpszClassName, window_title,
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left, wrc.bottom - wrc.top,
		nullptr, nullptr, window_class_.hInstance, nullptr
	);
	BOOST_ASSERT_MSG(hwnd_ != 0, "WindowInitializing failed");
}
