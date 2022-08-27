//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	/// <summary>
	/// CommandQueue�̓������s��Fence�̃��b�p�[
	/// </summary>
	class Fence final:public boost::noncopyable {
	public:
		Fence(ComPtr<ID3D12Device> device, UINT64 value0);
		void SetDebugName(LPCWSTR debug_name);
		/// <summary>
		/// fence��value�ȏ�ɂȂ����Ƃ��ɔ��΂���C�x���gevent���w�肷��
		/// </summary>
		void SetEventOnCompletion(UINT64 value, HANDLE event);
		/// <summary>
		/// fence��value���Z�b�g����
		/// </summary>
		void Signal(UINT64 value);
		/// <summary>
		/// fence�̌��݂̒l�𓾂�
		/// </summary>
		UINT64 GetCompletedValue() const;
		ID3D12Fence* GetRawPtr() const;
	private:
		ComPtr<ID3D12Fence> fence_;
	};
}