//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	/// <summary>
	/// CommandQueueの同期を行うFenceのラッパー
	/// </summary>
	class Fence final:public boost::noncopyable {
	public:
		Fence(ComPtr<ID3D12Device> device, UINT64 value0);
		void SetDebugName(LPCWSTR debug_name);
		/// <summary>
		/// fenceがvalue以上になったときに発火するイベントeventを指定する
		/// </summary>
		void SetEventOnCompletion(UINT64 value, HANDLE event);
		/// <summary>
		/// fenceにvalueをセットする
		/// </summary>
		void Signal(UINT64 value);
		/// <summary>
		/// fenceの現在の値を得る
		/// </summary>
		UINT64 GetCompletedValue() const;
		ID3D12Fence* GetRawPtr() const;
	private:
		ComPtr<ID3D12Fence> fence_;
	};
}