//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GraphicsPipeline.h"

#include "Log.h"

namespace{
	DXGI_FORMAT vertex_layout_format_correspond[(unsigned char)DX12::VertexLayoutFormat::size] = {
		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R32_UINT
	};
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive_topology_type_correspond[(unsigned char)DX12::PrimitiveTopology::size] = {
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
	};
	D3D12_PRIMITIVE_TOPOLOGY primitive_topology_correspond[(unsigned char)DX12::PrimitiveTopology::size] = {
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	};
}

DX12::GraphicsPipeline::GraphicsPipeline(ComPtr<ID3D12Device> device,
	std::shared_ptr<ShaderObject> vertex_shader, std::shared_ptr<ShaderObject> pixel_shader,
	VertexLayoutUnit* layout_unit_array, int layout_unit_num, bool dsbuffer,
	PrimitiveTopology topology, std::shared_ptr<RootSignature> root_signature, LPCWSTR debug_name)
	:primitive_topology_(topology)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = vertex_shader->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vertex_shader->GetBufferSize();
	gpipeline.PS.pShaderBytecode = pixel_shader->GetBufferPointer();
	gpipeline.PS.BytecodeLength = pixel_shader->GetBufferSize();
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	gpipeline.BlendState.IndependentBlendEnable = false;
	//HACK:αブレンドなどを行うならば変更
	CD3DX12_BLEND_DESC render_target_blend_desc(D3D12_DEFAULT);
	render_target_blend_desc.RenderTarget[0].BlendEnable = true;
	render_target_blend_desc.RenderTarget[0].LogicOpEnable = false;
	render_target_blend_desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	render_target_blend_desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	render_target_blend_desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	render_target_blend_desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
	render_target_blend_desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	render_target_blend_desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	render_target_blend_desc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	render_target_blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	gpipeline.BlendState = render_target_blend_desc;
	//頂点レイアウトの配列を変換
	D3D12_INPUT_ELEMENT_DESC* layouts = DBG_NEW D3D12_INPUT_ELEMENT_DESC[layout_unit_num];
	for (int n = 0; n < layout_unit_num; n++) {
		layouts[n] = {};
		layouts[n].SemanticName = layout_unit_array[n].semantic_name_;
		layouts[n].SemanticIndex = 0;
		layouts[n].Format = vertex_layout_format_correspond[(unsigned char)layout_unit_array[n].format_];
		layouts[n].InputSlot = layout_unit_array[n].input_slot_;
		layouts[n].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		layouts[n].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		layouts[n].InstanceDataStepRate = 0;
	}
	//Depth Stencil Bufferの設定
	if (dsbuffer) {
		gpipeline.DepthStencilState.DepthEnable = true;
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}
	else {
		gpipeline.DepthStencilState.DepthEnable = false;
	}
	gpipeline.InputLayout.pInputElementDescs = layouts;
	gpipeline.InputLayout.NumElements = layout_unit_num;
	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = primitive_topology_type_correspond[(unsigned char)primitive_topology_];
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;
	gpipeline.pRootSignature = root_signature->GetRawPtr();
	auto result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(state_.ReleaseAndGetAddressOf()));
	delete[] layouts;
	if (FAILED(result)) {
		Log::OutputTrivial("GraphicsPipeLine Initialising failed");
		throw 0;
	}
#ifdef _DEBUG
	state_->SetName(debug_name);
#endif
}

void DX12::GraphicsPipeline::SetGraphicsPipeline(ComPtr<ID3D12GraphicsCommandList> list)
{
	list->SetPipelineState(state_.Get());
	list->IASetPrimitiveTopology(primitive_topology_correspond[(unsigned char)primitive_topology_]);
}
