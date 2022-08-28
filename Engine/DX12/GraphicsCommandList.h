//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class DescriptorHeap;
	class Resource;
	class VertexBuffer;
	class IndexBuffer;
	class RootSignature;
	class GraphicsPipeline;
	class Buffer;
	class ShaderResource;
	/// <summary>
	/// resource barrier遷移の指定に使う
	/// </summary>
	struct ResourceBarrierUnit final{
	public:
		ResourceBarrierUnit(std::shared_ptr<Resource> r,D3D12_RESOURCE_STATES b,D3D12_RESOURCE_STATES a)
			:resource_(r),before_(b),after_(a)
		{}
		std::shared_ptr<Resource> resource_;
		D3D12_RESOURCE_STATES before_;
		D3D12_RESOURCE_STATES after_;
	};
	/// <summary>
	/// GraphicsCommandListとCommandAllocatorを管理する
	/// CommandListとAllocatorを1:1で対応させる
	/// </summary>
	class GraphicsCommandList final :public boost::noncopyable {
	public:
		//Masterから呼び出す
		GraphicsCommandList(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE cmdlist_type);
		void SetDebugName(LPCWSTR debug_name);
		/// <summary>
		/// desc_heap_のindex番目のRTVを指定色でクリア
		/// </summary>
		void ClearRenderTargetView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float r, float g, float b);
		/// <summary>
		/// desc_heap_のindex番目のDSVを指定値でクリア
		/// </summary>
		void ClearDepthStencilBufferView(std::shared_ptr<DescriptorHeap> desc_heap, int index, float depth_value);
		void SetResourceBarrier(ResourceBarrierUnit unit);
		void SetResourceBarrier(const std::vector<ResourceBarrierUnit>& units);
		/// <summary>
		/// Resource全体をコピーする
		/// </summary>
		void CopyResource(std::shared_ptr<Resource> dst, std::shared_ptr<Resource> src);
		/// <summary>
		/// Bufferの一部分をBufferへコピーする
		/// </summary>
		void CopyBufferRegion(std::shared_ptr<Resource> dst, UINT64 dst_offset,
			std::shared_ptr<Resource> src, UINT64 src_offset, UINT64 copy_size);
		void CopyBufferToTexture(std::shared_ptr<Buffer> buffer,
			UINT img_width,UINT img_height, DXGI_FORMAT img_format, UINT img_rowpitch,
			std::shared_ptr<ShaderResource> texture);
		/// <summary>
		/// num個のRTVをセットする。DSVはなし。
		/// RTVはdesc_heapのindex番目から連続して並んでいるとする。
		/// </summary>
		void SetRenderTargets(std::shared_ptr<DescriptorHeap> desc_heap, int index, int num);
		/// <summary>
		/// num個のRTVをセットする。DSVはdsv_desc_heapのdsv_index番目。
		/// RTVはrtv_desc_heapのrtv_index番目から連続して並んでいるとする。
		/// </summary>
		void SetRenderTargets(std::shared_ptr<DescriptorHeap> rtv_desc_heap, int rtv_index, int num,
			std::shared_ptr<DescriptorHeap> dsv_desc_heap, int dsv_index);
		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vert_buff, int slot_id = 0);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> index_buff);
		void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
		/// <summary>
		/// このDescriptorHeapをセットする
		/// (SetGraphicsRootDescriptorTableとは別に、用いるDescriptorHeapをセットする必要がある)
		/// </summary>
		void SetDescriptorHeap(std::shared_ptr<DescriptorHeap> desc_heap);
		/// <summary>
		/// このDescriptorHeap(複数個)をセットする
		/// (SetGraphicsRootDescriptorTableとは別に、用いるDescriptorHeapをセットする必要がある)
		/// </summary>
		void SetDescriptorHeaps(std::vector<std::shared_ptr<DescriptorHeap>> desc_heaps);
		/// <summary>
		/// このDescriptorHeapをRootParameterに対応させる
		/// </summary>
		/// <param name="root_param_index">何番目のRootParameterか</param>
		/// <param name="base_desc_heap_index">desc_heapの何番目のDescriptorからをセットするか</param>
		void SetGraphicsRootDescriptorTable(int root_param_index, std::shared_ptr<DescriptorHeap> desc_heap,
			int base_desc_heap_index = 0);
		/// <summary>
		/// このConstBufferをRootParameterに対応させる
		/// </summary>
		/// <param name="root_param_index">RootSignatureの何番目のRootParameterか</param>
		void SetGraphicsRootCBV(int root_param_index, std::shared_ptr<Buffer> const_buffer);
		/// <summary>
		/// srcから始まるデータをRootSignatureのroot_param_index番目のRootSignatureにセットする
		/// </summary>
		void SetGraphicsRootConstant(int root_param_index, SIZE_T size_to_set, void* src, int offset = 0);
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
		void SetViewports(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth);
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
		void DrawInstanced(int vertex_num, int start_vertex_loc = 0);
		/// <param name="index_num">描画するインデックスの数</param>
		/// <param name="start_index_loc">どのインデックスから描画するか</param>
		void DrawIndexedInstanced(int index_num, int start_index_loc = 0);
		/// <summary>
		/// CommandListをクローズする(recordを止める) executeの前に実行する
		/// </summary>
		void Close();
		void ResetCommandList();
		void ResetCommandAllocator();
		ID3D12GraphicsCommandList* GetCommandListRawPtr() const;
	private:
		ComPtr<ID3D12GraphicsCommandList> cmd_list_;
		ComPtr<ID3D12CommandAllocator> cmd_allocator_;
	};
}