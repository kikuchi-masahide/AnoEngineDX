//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

#include "ConfigEnums.h"
#include "VertexLayout.h"
#include "DescriptorRange.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace DX12 {
	class Master_pimple;
	class SwapChain;
	class ShaderObject;
	class VertexBuffer;
	class IndexBuffer;
	class RootSignature;
	class GraphicsPipeline;
	class ConstBuffer;
	class DescriptorHeap;
	class ShaderResource;
	class DepthStencilBuffer;
	class Master final:public boost::noncopyable
	{
	public:
		void Initialize();
		void ProcessCommands();
		/// <summary>
		/// このシェーダを読み込みShaderObjectを作成する
		/// </summary>
		std::shared_ptr<ShaderObject> LoadShader(LPCWSTR filename, DX12::ShaderType shader_type);
		std::shared_ptr<SwapChain> CreateSwapChain(HWND hwnd, UINT width, UINT height);
		/// <summary>
		/// 頂点バッファを作る
		/// </summary>
		/// <param name="size_per_vert">1頂点を表す構造体のサイズ</param>
		/// <param name="vert_num">全頂点数</param>
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(SIZE_T size_per_vert, int vert_num, LPCWSTR debug_name);
		/// <summary>
		/// インデックスバッファを作る
		/// </summary>
		/// <param name="vert_num">配列に含まれる総頂点数(三角形のポリゴン一つに付き三頂点)</param>
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(int vert_num, LPCWSTR debug_name);
		/// <summary>
		/// 定数バッファを作る
		/// </summary>
		/// <param name="size">バッファ全体のサイズ(自動的に256アラインメントする)</param>
		/// <param name="heap_type">アクセスタイプ</param>
		std::shared_ptr<ConstBuffer> CreateConstBuffer(SIZE_T size, ResourceHeapType heap_type, LPCWSTR debug_name);
		/// <summary>
		/// Depth Stencil Bufferを作る
		/// </summary>
		/// <param name="width">px単位での幅(レンダーターゲットに合わせる)</param>
		/// <param name="height">px単位での高さ(レンダーターゲットに合わせる)</param>
		/// <param name="heap_type">Heap Type(通常GPUからしか操作しないためDEFAULT)</param>
		std::shared_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(UINT64 width, UINT64 height, ResourceHeapType heap_type = ResourceHeapType::DEFAULT);
		/// <summary>
		/// DescriptorHeapを作る
		/// </summary>
		/// <param name="capacity">このヒープが持つDescriptorの数</param>
		/// <param name="type">格納するDescriptorの種別</param>
		/// <param name="vis">Shaderからの可視性</param>
		std::shared_ptr<DescriptorHeap> CreateDescriptorHeap(int capacity, DescriptorHeapType type, DescriptorHeapShaderVisibility vis, LPCWSTR debug_name);
		/// <summary>
		/// このDescriptorHeapに、const_buffのCBVを作成する
		/// </summary>
		/// <param name="index">desc_heapの何番目(0-indexed)に作成するか</param>
		void CreateConstBufferView(std::shared_ptr<ConstBuffer> const_buff, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// このDescriptorHeapに、shader_resourceのRTVを作成する
		/// </summary>
		/// <param name="index">desc_heapの何番目(0-indexed)に作成するか</param>
		void CreateRenderTargetView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// このDescriptorHeapに、shader_resourceのSRVを作成する
		/// </summary>
		/// <param name="index">desc_heapの何番目(0-indexed)に作成するか</param>
		void CreateShaderResourceView(std::shared_ptr<ShaderResource> shader_resource, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		/// <summary>
		/// このDesciptorHeapに、dsbufferのDSVを作成する
		/// </summary>
		/// <param name="index">desc_heapの何番目(0-indexed)に作成するか</param>
		void CreateDepthStencilBufferView(std::shared_ptr<DepthStencilBuffer> dsbuffer, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		//1つのルートシグネチャを表すオブジェクトを作成する
		std::shared_ptr<RootSignature> CreateRootSignature(int root_param_num);
		/// <summary>
		/// グラフィックパイプラインを表すオブジェクトを作成する
		/// </summary>
		/// <param name="dsbuffer">Depth Stencil Bufferを有効にするか否か</param>
		/// <param name="topology">このGraphics Pipelineで描画する形態</param>
		std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(
			std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
			VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer,
			PrimitiveTopology topology, std::shared_ptr<RootSignature> root_signature, LPCWSTR name);
		/// <summary>
		/// このファイルを読み込み、ファイルデータを保有するShaderResourceを返す
		/// </summary>
		/// <param name="debug_name">コピー前/後のバッファのデバッグ用名前</param>
		std::shared_ptr<ShaderResource> LoadTexture(LPCWSTR filename, LPCWSTR debug_name);
		/// <summary>
		/// swapchainの今のバックバッファにバリアを張る
		/// </summary>
		void SetResourceBarrier(std::shared_ptr<SwapChain> swapchain,
			ResourceBarrierState before, ResourceBarrierState after);
		/// <summary>
		/// ShaderResourceにバリアを張る
		/// </summary>
		void SetResourceBarrier(std::shared_ptr<ShaderResource> shader_resource,
			ResourceBarrierState before, ResourceBarrierState after);
		/// <summary>
		/// このRootSignatureに、DescriptorTableタイプのRootParameterを追加する
		/// </summary>
		void AddRootParameterAsDescriptorTable(std::shared_ptr<RootSignature> root_signature,
			std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis);
		/// <summary>
		/// この頂点バッファに情報をコピーするためのポインタを取得する
		/// </summary>
		void* Map(std::shared_ptr<VertexBuffer> vert_buffer);
		/// <summary>
		/// このインデックスバッファに情報をコピーするためのポインタを取得する
		/// </summary>
		unsigned int* Map(std::shared_ptr<IndexBuffer> index_buffer);
		void* Map(std::shared_ptr<ConstBuffer> const_buffer);
		void Unmap(std::shared_ptr<VertexBuffer> vert_buffer);
		void Unmap(std::shared_ptr<IndexBuffer> index_buffer);
		void Unmap(std::shared_ptr<ConstBuffer> const_buffer);
		/// <summary>
		/// ルートシグネチャをシリアライズする 以降ルートパラメタやサンプラーの変更はできない
		/// </summary>
		void Serialize(std::shared_ptr<RootSignature> root_signature);
		/// <summary>
		/// コマンドリストに今のバックバッファのクリア命令を投げる
		/// </summary>
		void ClearRenderTarget(std::shared_ptr<SwapChain> swapchain, float r, float g, float b);
		/// <summary>
		/// desc_heapのindex番目のDSVの指すDepth Stencil Bufferを、depth_valueでクリアする
		/// </summary>
		void ClearDepthStencilView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value = 1.0f);
		/// <summary>
		/// swapchainの今のバックバッファをレンダーターゲットに指定する(Depth Stencil Bufferをセットしない)
		/// </summary>
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain);
		/// <summary>
		/// swapchainの今のバックバッファをレンダーターゲットに指定する
		/// (desc_heapのindex番目に存在するDSVが指すDepth Stencil Bufferをセットする)
		/// </summary>
		void SetRenderTarget(std::shared_ptr<SwapChain> swapchain, std::shared_ptr<DescriptorHeap> desc_heap, int index);
		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vert_buffer, int slot_id = 0);
		/// <summary>
		/// このIndexBufferをインデックスバッファとしてセットする
		/// </summary>
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer);
		/// <summary>
		/// このRootSignatureをセットする
		/// </summary>
		void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
		/// <summary>
		/// このDescriptorHeapをセットする
		/// (SetGraphicsRootDescriptorTableとは別に、用いるDescriptorHeapをセットする必要がある)
		/// </summary>
		/// <param name="desc_heap"></param>
		void SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap);
		/// <summary>
		/// このDescriptorHeapをRootParameterに対応させる
		/// </summary>
		/// <param name="root_param_index">何番目のRootParameterか</param>
		/// <param name="base_desc_heap_index">desc_heapの何番目のDescriptorからをセットするか</param>
		void SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap, int base_desc_heap_index = 0);
		/// <summary>
		/// このGraphicsPipelineとそれの従うPrimitiveTopologyをセットする
		/// </summary>
		void SetGraphicsPipeline(std::shared_ptr<GraphicsPipeline> pipeline);
		/// <summary>
		/// レンダーターゲットのうち、どの長方形部分に視錐台を描画するか指定する
		/// </summary>
		/// <param name="top_left_x">レンダーターゲット左上を(0,0)としたときの、px単位での左x座標</param>
		/// <param name="top_left_y">レンダーターゲット左上を(0,0)としたときの、px単位での上y座標</param>
		/// <param name="width">px単位での幅</param>
		/// <param name="height">px単位での高さ</param>
		/// <param name="min_depth">近接平面のz座標</param>
		/// <param name="max_depth">遠平面のz座標</param>
		void SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth = 0.0f, float max_depth = 1.0f);
		/// <summary>
		/// レンダーターゲットのうち、どの長方形部分をシザーレクトにするか指定する
		/// シザーレクト外のピクセルは捨てられる
		/// </summary>
		/// <param name="top_left_x">左上を(0,0)としたときの、px単位での左x座標</param>
		/// <param name="top_left_y">左上を(0,0)としたときの、px単位での上y座標</param>
		/// <param name="bottom_right_x">左上を(0,0)としたときの、px単位での右x座標</param>
		/// <param name="bottom_right_y">左上を(0,0)としたときの、px単位での下y座標</param>
		void SetScissorRect(LONG top_left_x, LONG top_left_y, LONG bottom_right_x, LONG bottom_right_y);
		/// <param name="vertex_num">描画する頂点数</param>
		/// <param name="start_vertex_loc">どの頂点から描画するか</param>
		void DrawInstanced(int vertex_num, int start_vertex_loc);
		/// <param name="index_num">描画するインデックスの数</param>
		/// <param name="start_index_loc">どのインデックスから描画するか</param>
		void DrawIndexedInstanced(int index_num, int start_index_loc);
		/// <summary>
		/// このswapchainをflipする
		/// </summary>
		void Flip(std::shared_ptr<SwapChain> swapchain);
		void CleanUp();
	private:
		Master_pimple* pimple_;
	};
}

