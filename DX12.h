#pragma once

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")

class DX12Factory;
class DX12Device;
class DX12CmdAllocator;
class DX12CmdList;
class DX12CmdQueue;
class SwapChainManager;

/// <summary>
/// Game�Ɋ܂܂��CDirectX12�����b�v�����N���X
/// </summary>
class DX12:public boost::noncopyable
{
private:
	DX12Factory* mFactory;
	DX12Device* mDevice;
	DX12CmdAllocator* mCmdAllocator;
	DX12CmdList* mCmdList;
	DX12CmdQueue* mCmdQueue;
	SwapChainManager* mSwapChainManager;
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