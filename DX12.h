#pragma once

class DX12Pimple;

/// <summary>
/// Game�Ɋ܂܂��CDirectX12�����b�v�����N���X
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
	/// �X���b�v�`�F�[�������
	/// </summary>
	/// <returns>�X���b�v�`�F�[���̃L�[</returns>
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
};