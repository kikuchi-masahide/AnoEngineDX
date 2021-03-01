#pragma once

#include "DX12ConfigEnums.h"
#include "DX12VertexLayoutUnit.h"
#include "DX12RootParameter.h"
class DX12Resource;
class DX12DescriptorHeap;
class DX12ShaderObject;
class DX12GraphicsPipeline;
class DX12RootSignature;
class DX12SwapChain;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3dcompiler.lib")

class DX12Pimple;

class DX12 :public boost::noncopyable {
private:
	DX12Pimple* mPimple;
public:
	DX12();
	void Initialize();
	void CleanUp();
	~DX12();
	/// <summary>
	/// ディスクリプタヒープを作る
	/// </summary>
	/// <param name="_type">ディスクリプタヒープの種別</param>
	/// <param name="_vis">シェーダから可視か否か</param>
	/// <param name="_num">所有ディスクリプタの数</param>
	/// <returns>ディスクリプタヒープを表すshared_ptr</returns>
	boost::shared_ptr<DX12DescriptorHeap> CreateDescriptorHeap(DX12Config::DescriptorHeapType _type, DX12Config::DescriptorHeapShaderVisibility _vis, unsigned int _num);
	/// <summary>
	/// 各フレームで必要なコマンドを追加しきったあと呼び出す
	/// </summary>
	void ProcessCommands();
	/// <summary>
	/// 頂点バッファの作成
	/// </summary>
	/// <param name="_width">頂点バッファの全サイズ</param>
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
	boost::shared_ptr<DX12RootSignature> CreateRootSignature(DX12RootParameter& _rootparam);
	//パイプラインステートのセット
	void SetGraphicsPipeline(boost::shared_ptr<DX12GraphicsPipeline> _pipeline);
	//ルートシグネチャのセット
	void SetRootSignature(boost::shared_ptr<DX12RootSignature> _root);
	//プリミティブのコマンドリストへのセット
	void SetPrimitiveTopology(DX12Config::PrimitiveTopology _prim);
	/// <summary>
	/// 頂点バッファをコマンドリストへセット
	/// </summary>
	/// <param name="_allsize">頂点バッファ全体のサイズ</param>
	/// <param name="_sizepervertex">1頂点バッファあたりのサイズ</param>
	void SetVertexBuffers(boost::shared_ptr<DX12Resource> _resource, unsigned int _slotid,
		SIZE_T _allsize, SIZE_T _sizepervertex);
	/// <summary>
	/// コマンドリストへ描画命令を投げる
	/// </summary>
	/// <param name="_indexnumperinst">インスタンスごとの，頂点インデックス数</param>
	/// <param name="_instnum">インスタンス数</param>
	/// <param name="_indoffset">頂点インデックスのオフセット</param>
	/// <param name="_vdataoffset">頂点データのオフセット</param>
	/// <param name="_instoffset">インスタンスのオフセット</param>
	void DrawIndexedInstanced(UINT _indexnumperinst, UINT _instnum, UINT _indoffset,
			UINT _vdataoffset, UINT _instoffset);
	/// <summary>
	/// ビューポートのセット
	/// </summary>
	/// <param name="_widthpx">幅のpx</param>
	/// <param name="_heightpx">高さのpx</param>
	void SetViewports(UINT _widthpx, UINT _heightpx, int _topleftx, int _toplefty,
		float _maxdepth, float _mindepth);
	/// <summary>
	/// シザー矩形のセット
	/// </summary>
	void SetScissorrect(float _top, float _bottom, float _left, float _right);
	/// <summary>
	/// 対象リソースを頂点インデックスバッファに指定
	/// </summary>
	/// <param name="_vertnum">インデックスバッファの含む全頂点数</param>
	void SetIndexBuffers(boost::shared_ptr<DX12Resource> _resource,unsigned int _vertnum);
	/// <summary>
	/// インデックスバッファの作成
	/// </summary>
	/// <param name="_size">総頂点数</param>
	boost::shared_ptr<DX12Resource> CreateIndexBuffer(unsigned int _vertnum);
	/// <summary>
	/// テクスチャを読み込みディスクリプタを作成
	/// </summary>
	/// <param name="_desc">ディスクリプタヒープ</param>
	/// <param name="_num">ディスクリプタヒープの何番目にディスクリプタを作成するか</param>
	/// <returns>GPU上の読み取り専用バッファを示すDX12Resource</returns>
	boost::shared_ptr<DX12Resource> LoadTexture(const wchar_t* _filename, boost::shared_ptr<DX12DescriptorHeap> _desc,unsigned int _num);
	/// <summary>
	/// コマンドリストに対するディスクリプタヒープの指定
	/// </summary>
	void SetDescriptorHeap(boost::shared_ptr<DX12DescriptorHeap> _descHeap);
	/// <summary>
	/// ディスクリプタテーブルの設定
	/// </summary>
	/// <param name="_rootParamIndex">何番のルートパラメータを設定するか</param>
	/// <param name="_descHeap">ディスクリプタヒープ</param>
	/// <param name="_descHeapIndex">ディスクリプタヒープの何番を設定するか</param>
	void SetGraphicsRootDescriptorTable(unsigned int _rootParamIndex, boost::shared_ptr<DX12DescriptorHeap> _descHeap, unsigned int _descHeapIndex);
	/// <summary>
	/// リソースバリアをリストにセット
	void SetResourceBarrier(boost::shared_ptr<DX12Resource> _resource, DX12Config::ResourceBarrierState _before, DX12Config::ResourceBarrierState _after);
	boost::shared_ptr<DX12SwapChain> CreateSwapChain(HWND _hwnd, UINT _width, UINT _height);
	/// <summary>
	/// スワップチェーンのフリップ
	/// </summary>
	void FlipSwapChain(boost::shared_ptr<DX12SwapChain> _swapchain);
	/// <summary>
	/// レンダーターゲットを開く(リソースバリアの設定込み)
	/// </summary>
	void OpenRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain);
	/// <summary>
	/// レンダーターゲットを閉じる(リソースバリアの設定込み)
	/// </summary>
	void CloseRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain);
	/// <summary>
	/// 指定色でレンダーターゲットをクリア
	/// </summary>
	void ClearRenderTarget(boost::shared_ptr<DX12SwapChain> _swapchain, float _r, float _g, float _b);
	/// <summary>
	/// 指定の大きさ・色を持つ空のテクスチャを作成(初期状態はPIXEL_SHADER_RESOURCE)
	/// </summary>
	boost::shared_ptr<DX12Resource> CreateClearTexture(UINT64 _width, UINT64 _height, float _r, float _g, float _b, float _alpha);
	/// <summary>
/// 指定ディスクリプタヒープ上にこのリソースのレンダーターゲットビューを作る
/// </summary>
/// <param name="_n">ディスクリプタヒープの何番目にビューを作るか</param>
	void CreateRenderTargetView(boost::shared_ptr<DX12Resource> _resource, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n);
	/// <summary>
	/// 指定ディスクリプタヒープ上にこのリソースのレンダーターゲットビューを作る
	/// </summary>
	/// <param name="_n">ディスクリプタヒープの何番目にビューを作るか</param>
	void CreateShaderResourceView(boost::shared_ptr<DX12Resource> _resource, boost::shared_ptr<DX12DescriptorHeap> _descheap, int _n);
	//ディスクリプタヒープとIDを指定してレンダーターゲットをopen(バリアの設定は無し)
	void OpenRenderTarget(boost::shared_ptr<DX12DescriptorHeap> _heap, unsigned int _id);
	void ClearRenderTarget(boost::shared_ptr<DX12DescriptorHeap> _heap, unsigned int _id, float _r, float _g, float _b, float _alpha);
};