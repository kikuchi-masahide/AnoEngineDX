#pragma once

#include "DX12ConfigEnums.h"
#include "DX12VertexLayoutUnit.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3dcompiler.lib")

class DX12Factory;
class DX12Device;
class DX12CmdAllocator;
class DX12CmdList;
class DX12CmdQueue;
class SwapChainManager;
class DX12DescriptorHeap;
class DX12Resource;
class DX12ShaderObject;
class DX12GraphicsPipeline;
class DX12RootSignature;

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
	//シェーダ読み込み
	boost::shared_ptr<DX12ShaderObject> LoadShader(LPCWSTR _filename, DX12Config::ShaderType _shaderType);
	/// <summary>
	/// グラフィックスパイプラインの作成
	/// </summary>
	/// <param name="_vertexShader">頂点シェーダ</param>
	/// <param name="_pixelShader">ピクセルシェーダ</param>
	/// <param name="_vertexLayout">頂点レイアウト</param>
	/// <param name="_primitive">プリミティブ型</param>
	/// <param name="_numrt">レンダーターゲット数</param>
	boost::shared_ptr<DX12GraphicsPipeline> CreateGraphicsPipeline(
		boost::shared_ptr<DX12ShaderObject> _vertexShader,
		boost::shared_ptr<DX12ShaderObject> _pixelShader, DX12VertexLayout& _vertexLayout,
		DX12Config::PrimitiveTopologyType _primitive, UINT _numrt,
		boost::shared_ptr<DX12RootSignature> _rootsignature);
	//ルートシグネチャの作成(暫定)
	boost::shared_ptr<DX12RootSignature> CreateRootSignature();
};