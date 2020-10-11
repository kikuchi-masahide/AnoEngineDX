#include "window.h"
#include <boost/lexical_cast.hpp>

Window::Window(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height, LPCWSTR _windowTitle)
{
	Create(_wndproc, _classID, _width, _height, _windowTitle);
}

Window::Window() :mHwnd(NULL), mWindowClass({}) {}

void Window::Create(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height, LPCWSTR _windowTitle)
{
	if (mHwnd != NULL)
	{
		Log::OutputCritical("recreating window");
		assert(0);
	}
	RECT wrc = {};
	wrc.left = 0;
	wrc.top = 0;
	wrc.right = _width;
	wrc.bottom = _height;
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	mWindowClass.hInstance = GetModuleHandle(nullptr);
	mWindowClass.cbSize = sizeof(WNDCLASSEX);
	mWindowClass.lpfnWndProc = (WNDPROC)_wndproc;
	mWindowClass.lpszClassName = _classID;
	RegisterClassEx(&mWindowClass);
	mHwnd = CreateWindow(
		mWindowClass.lpszClassName, _windowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left, wrc.bottom - wrc.top,
		nullptr, nullptr, mWindowClass.hInstance, nullptr
	);
	if (mHwnd == 0) {
		std::string errmsg("WindowInitializing failed");
		Log::OutputCritical(errmsg);
		assert(0);
	}
}

Window::~Window()
{
	UnregisterClass(mWindowClass.lpszClassName, mWindowClass.hInstance);
}

void Window::ShowWindow()
{
	::ShowWindow(mHwnd, SW_SHOW);
}

HWND Window::GetWindowHandle() const
{
	return mHwnd;
}

RECT Window::GetWindowSize() const
{
	RECT rect;
	GetClientRect(mHwnd, &rect);
	return rect;
}
