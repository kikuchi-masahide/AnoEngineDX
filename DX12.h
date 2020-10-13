#pragma once

class DX12Pimple;

/// <summary>
/// Gameに含まれる，DirectX12をラップしたクラス
/// </summary>
class DX12:public boost::noncopyable
{
private:
	DX12Pimple* mPimple;
public:
	DX12();
	void Initialize();
	void CleanUp();
	/// <summary>
	/// スワップチェーンを作る
	/// </summary>
	/// <returns>スワップチェーンのキー</returns>
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
};