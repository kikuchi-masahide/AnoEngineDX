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
};