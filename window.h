#pragma once
#include <Windows.h>

class Window {
private:
	WNDCLASSEX mWindowClass;
	HWND mHwnd;
public:
	/// <summary>
	/// 一つのウィンドウを表すクラス
	/// </summary>
	/// <param name="_wndproc">ウィンドウプロシャ―ジャ</param>
	/// <param name="_classID">ウィンドウクラスを表すID</param>
	/// <param name="_width">幅</param>
	/// <param name="_height">高さ</param>
	/// <param name="_windowTitle">ウィンドウのタイトル</param>
	Window(WNDPROC _wndproc, LPCWSTR _classID,int _width,int _height,
		LPCWSTR _windowTitle);
	HWND GetWindowHandle() const;
	RECT GetWindowSize() const;
	~Window();
};