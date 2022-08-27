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
	class GraphicsPipeline final:public boost::noncopyable{
	public:
		GraphicsPipeline(ComPtr<ID3D12Device> device,
			std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
			const std::vector<VertexLayoutUnit>& vertex_layout, bool dsbuffer,
			D3D_PRIMITIVE_TOPOLOGY primitive_topology, std::shared_ptr<RootSignature> root_signature);
		void SetDebugName(LPCWSTR debug_name);
		ID3D12PipelineState* GetRawPtr() const;
		D3D_PRIMITIVE_TOPOLOGY const primitive_topology_;
	private:
		ComPtr<ID3D12PipelineState> state_;
	};
}