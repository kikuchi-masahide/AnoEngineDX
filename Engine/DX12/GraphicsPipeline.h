//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ShaderObject.h"
#include "VertexLayout.h"
#include "RootSignature.h"
#include "DepthStencilBuffer.h"

namespace DX12 {
	class GraphicsPipeline final:public boost::noncopyable
	{
	public:
		GraphicsPipeline(ComPtr<ID3D12Device> device,
			std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
			VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer,
			PrimitiveTopology topology, std::shared_ptr<RootSignature> root_signature, LPCWSTR name);
		//パイプラインの設定と同時に、SetPrimitiveTopologyも行う
		void SetGraphicsPipeline(ComPtr<ID3D12GraphicsCommandList> list);
	private:
		ComPtr<ID3D12PipelineState> state_;
		PrimitiveTopology primitive_topology_;
	};
}
