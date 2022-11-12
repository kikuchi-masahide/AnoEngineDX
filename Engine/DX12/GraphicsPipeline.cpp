//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GraphicsPipeline.h"

#include "ShaderObject.h"
#include "RootSignature.h"
#include "Log.h"

DX12::GraphicsPipeline::GraphicsPipeline()
{
}

DX12::GraphicsPipeline::GraphicsPipeline(ComPtr<ID3D12Device> device,
	ShaderObject vertex_shader, ShaderObject pixel_shader,
	const std::vector<VertexLayoutUnit>& vertex_layout, bool dsbuffer,
	D3D_PRIMITIVE_TOPOLOGY primitive_topology, RootSignature root_signature)
	:primitive_topology_(primitive_topology)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = vertex_shader.GetBufferPointer();
	gpipeline.VS.BytecodeLength = vertex_shader.GetBufferSize();
	gpipeline.PS.pShaderBytecode = pixel_shader.GetBufferPointer();
	gpipeline.PS.BytecodeLength = pixel_shader.GetBufferSize();
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
	D3D12_INPUT_ELEMENT_DESC* layouts = DBG_NEW D3D12_INPUT_ELEMENT_DESC[vertex_layout.size()];
	for (int n = 0; n < vertex_layout.size(); n++) {
		layouts[n] = {};
		layouts[n].SemanticName = vertex_layout[n].semantic_name_;
		layouts[n].SemanticIndex = 0;
		layouts[n].Format = vertex_layout[n].format_;
		layouts[n].InputSlot = vertex_layout[n].input_slot_;
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
	gpipeline.InputLayout.NumElements = vertex_layout.size();
	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = ConvertToTopologyType(primitive_topology_);
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;
	gpipeline.pRootSignature = root_signature.GetRawPtr();
	auto result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(state_.ReleaseAndGetAddressOf()));
	delete[] layouts;
	if (FAILED(result)) {
		Log::OutputTrivial("GraphicsPipeLine Initialising failed");
		throw 0;
	}
}

void DX12::GraphicsPipeline::SetDebugName(LPCWSTR debug_name)
{
#ifdef _DEBUG
	state_->SetName(debug_name);
#endif
}

bool DX12::GraphicsPipeline::IsValid() const
{
	return state_;
}

D3D_PRIMITIVE_TOPOLOGY DX12::GraphicsPipeline::GetPrimitiveTopology() const
{
	return primitive_topology_;
}

ID3D12PipelineState* DX12::GraphicsPipeline::GetRawPtr() const
{
	return state_.Get();
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE DX12::ConvertToTopologyType(D3D12_PRIMITIVE_TOPOLOGY primitive_topology)
{
	switch (primitive_topology) {
	case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}
}
