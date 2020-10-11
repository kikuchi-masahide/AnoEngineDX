#pragma once
#include <Windows.h>

/// <summary>
/// 1つのウィンドウを表すクラス(コピー不可)
/// </summary>
class Window : public boost::noncopyable {
private:
	WNDCLASSEX mWindowClass;
	HWND mHwnd;
public:
	/// <summary>
	/// ウィンドウの作成を同時に行うコンストラクタ
	/// </summary>
	/// <param name="_wndproc">ウィンドウプロシャ―ジャ</param>
	/// <param name="_classID">ウィンドウクラスを表すID</param>
	/// <param name="_width">幅</param>
	/// <param name="_height">高さ</param>
	/// <param name="_windowTitle">ウィンドウのタイトル</param>
	Window(WNDPROC _wndproc, LPCWSTR _classID,int _width,int _height,
		LPCWSTR _windowTitle);
	Window();
	/// <summary>
	/// ウィンドウを作成する
	/// </summary>
	/// <param name="_wndproc">ウィンドウプロシャ―ジャ</param>
	/// <param name="_classID">ウィンドウクラスを表すID</param>
	/// <param name="_width">幅</param>
	/// <param name="_height">高さ</param>
	/// <param name="_windowTitle">ウィンドウのタイトル</param>
	void Create(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height,
		LPCWSTR _windowTitle);
	HWND GetWindowHandle() const;
	RECT GetWindowSize() const;
	~Window();
	void ShowWindow();
};