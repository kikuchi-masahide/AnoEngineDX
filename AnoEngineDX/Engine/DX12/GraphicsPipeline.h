//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "VertexLayout.h"

namespace DX12 {
	class ShaderObject;
	class RootSignature;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertToTopologyType(D3D12_PRIMITIVE_TOPOLOGY primitive_topology);
	class GraphicsPipeline final {
	public:
		GraphicsPipeline();
		GraphicsPipeline(ComPtr<ID3D12Device> device,
			ShaderObject vertex_shader, ShaderObject pixel_shader,
			const std::vector<VertexLayoutUnit>& vertex_layout, bool dsbuffer,
			D3D_PRIMITIVE_TOPOLOGY primitive_topology, RootSignature root_signature);
		void SetDebugName(LPCWSTR debug_name);
		bool IsValid() const;
		D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
		ID3D12PipelineState* GetRawPtr() const;
	private:
		D3D_PRIMITIVE_TOPOLOGY primitive_topology_;
		ComPtr<ID3D12PipelineState> state_;
	};
}