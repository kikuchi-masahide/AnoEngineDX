//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DescriptorRange.h"

namespace DX12 {
	class RootSignature final: public boost::noncopyable{
	public:
		RootSignature(int root_param_num);
		void Serialize(ComPtr<ID3D12Device> device);
		void AddRootParameterAsDescriptorTable(std::vector<DescriptorRange>& ranges,
			D3D12_SHADER_VISIBILITY vis);
		void AddRootParameterAsCBV(UINT shader_register, D3D12_SHADER_VISIBILITY vis);
		void AddRootParameterAsSRV(UINT shader_register, D3D12_SHADER_VISIBILITY vis);
		void AddRootParameterAsConstant(UINT shader_register, SIZE_T const_size,
			D3D12_SHADER_VISIBILITY vis);
		ID3D12RootSignature* GetRawPtr() const;
	private:
		ComPtr<ID3D12RootSignature> root_signature_;
		bool serialized_;
		std::vector<CD3DX12_ROOT_PARAMETER> root_params_;
	};
}