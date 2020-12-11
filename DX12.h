#pragma once

#include "DX12ConfigEnums.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")

class DX12Factory;
class DX12Device;
class DX12CmdAllocator;
class DX12CmdList;
class DX12CmdQueue;
class SwapChainManager;
class DX12DescriptorHeap;
class DX12Resource;

/// <summary>
/// Gameに含まれる，DirectX12をラップしたクラス
/// </summary>
class DX12 :public boost::noncopyable
{
private:
	DX12Factory* mFactory;
	DX12Device* mDevice;
	DX12CmdAllocator* mCmdAllocator;
	DX12CmdList* mCmdList;
	DX12CmdQueue* mCmdQueue;
	SwapChainManager* mSwapChainManager;
	void EnableDebugLayer();
	//ProcessCommandsに含まれるフェンスによる待ち時間の部分
	void FenceWaitingInProcessCommands();
public:
	DX12();
	void Initialize();
	void CleanUp();
	/// <summary>
	/// スワップチェーンを作る
	/// </summary>
	/// <param name="_descheap">スワップチェーンと紐づけるディスクリプタヒープ</param>
	/// <returns>スワップチェーンのキー</returns>
	unsigned int CreateSwapChain(HWND _hwnd, UINT _width, UINT _height, boost::shared_ptr<DX12DescriptorHeap> _descheap);
	/// <summary>
	/// ディスクリプタヒープを作る
	/// </summary>
	/// <param name="_type">ディスクリプタヒープの種別</param>
	/// <param name="_vis">シェーダから可視か否か</param>
	/// <param name="_num">所有ディスクリプタの数</param>
	/// <returns>ディスクリプタヒープを表すshared_ptr</returns>
	boost::shared_ptr<DX12DescriptorHeap> CreateDescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::ShaderVisibility _vis, unsigned int _num);
	/// <summary>
	/// 各フレームで必要なコマンドを追加しきったあと呼び出す
	/// </summary>
	void ProcessCommands();
	/// <summary>
	/// _idの指すスワップチェーンをレンダーターゲットに指定し背景色塗りつぶし
	/// </summary>
	void SetAndClearRenderTarget(unsigned int _id,float _r,float _g,float _b);
	//頂点バッファ作成
	boost::shared_ptr<DX12Resource> CreateVertexBuffer(UINT64 _width);
	//リソースのマップ
	void* Map(boost::shared_ptr<DX12Resource> _resource);
	void Unmap(boost::shared_ptr<DX12Resource> _resource);
};