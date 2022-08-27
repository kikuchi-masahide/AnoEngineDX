//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class GraphicsCommandList;
	class Fence;
	class CommandQueue final:public boost::noncopyable{
	public:
		CommandQueue(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE cmdlist_type);
		void SetDebugName(LPCWSTR debug_name);
		/// <summary>
		/// command listのコマンドを実行する
		/// </summary>
		void ExecuteCommandLists(const std::vector<std::shared_ptr<GraphicsCommandList>>& lists);
		/// <summary>
		/// コマンドの実行を完了したのち、このfenceにvalueをセットする
		/// </summary>
		void Signal(std::shared_ptr<Fence> fence, UINT64 value);
		/// <summary>
		/// fenceがvalue以上になるまで待つ
		/// </summary>
		void Wait(std::shared_ptr<Fence> fence, UINT64 value);
		ID3D12CommandQueue* GetRawPtr() const;
	private:
		ComPtr<ID3D12CommandQueue> cmd_queue_;
	};
}