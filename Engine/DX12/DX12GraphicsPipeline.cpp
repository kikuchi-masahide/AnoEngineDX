#include "DX12GraphicsPipeline.h"
#include "DX12ShaderObject.h"
#include "DX12RootSignature.h"
#include "DX12Pimple.h"
#include "../Log.h"

DX12GraphicsPipeline::DX12GraphicsPipeline(
	ComPtr<ID3D12Device> _device, boost::shared_ptr<DX12ShaderObject> _vertexShader,
	boost::shared_ptr<DX12ShaderObject> _pixelShader, DX12VertexLayout& _vertexLayout,
	DX12Config::PrimitiveTopologyType _primitive, UINT _numrt,
	boost::shared_ptr<DX12RootSignature> _rootsignature, LPCWSTR _name)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = _vertexShader->GetBufferPointer();
	gpipeline.VS.BytecodeLength = _vertexShader->GetBufferSize();
	gpipeline.PS.pShaderBytecode = _pixelShader->GetBufferPointer();
	gpipeline.PS.BytecodeLength = _pixelShader->GetBufferSize();
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	gpipeline.BlendState.IndependentBlendEnable = false;
	CD3DX12_BLEND_DESC renderTargetBlendDesc(D3D12_DEFAULT);
	//αブレンドを行う
	renderTargetBlendDesc.RenderTarget[0].BlendEnable = true;
	renderTargetBlendDesc.RenderTarget[0].LogicOpEnable = false;
	renderTargetBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
	renderTargetBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	renderTargetBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	renderTargetBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	gpipeline.BlendState = renderTargetBlendDesc;
	//頂点レイアウトの配列をこちらで作り直す
	unsigned int layoutNum = _vertexLayout.size();
	D3D12_INPUT_ELEMENT_DESC* layouts = DBG_NEW D3D12_INPUT_ELEMENT_DESC[layoutNum];
	for (unsigned int n = 0; n < layoutNum; n++) {
		layouts[n] = {};
		layouts[n].SemanticName = _vertexLayout[n].mSemanticName;
		layouts[n].SemanticIndex = 0;
		layouts[n].Format = mVertexLayoutFormatCorrespond[(unsigned char)_vertexLayout[n].mFormat];
		layouts[n].InputSlot = _vertexLayout[n].mInputSlot;
		layouts[n].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		layouts[n].InputSlotClass = mVertexLayoutInputClassificationCorrespond[(unsigned char)_vertexLayout[n].mInputClassification];
		layouts[n].InstanceDataStepRate = _vertexLayout[n].mInstanceDataStepRate;
	}
	gpipeline.InputLayout.pInputElementDescs = layouts;
	gpipeline.InputLayout.NumElements = layoutNum;
	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = mPrimitiveTopologyTypeCorrespond[(unsigned char)_primitive];
	gpipeline.NumRenderTargets = _numrt;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;
	gpipeline.pRootSignature = _rootsignature->mRootSignature.Get();
	auto result = _device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(mState.ReleaseAndGetAddressOf()));
	delete[] layouts;
	if (FAILED(result))
	{
		Log::OutputTrivial("GraphicsPipeLine Initialising failed");
		throw 0;
	}
#ifdef _DEBUG
	mState->SetName(_name);
#endif
}

void DX12GraphicsPipeline::SetGraphicsPipeline(ComPtr<ID3D12GraphicsCommandList> _list)
{
	_list->SetPipelineState(mState.Get());
}

boost::shared_ptr<DX12GraphicsPipeline> DX12Pimple::CreateGraphicsPipeline(
	boost::shared_ptr<DX12ShaderObject> _vertexShader,
	boost::shared_ptr<DX12ShaderObject> _pixelShader, DX12VertexLayout& _vertexLayout,
	DX12Config::PrimitiveTopologyType _primitive, UINT _numrt,
	boost::shared_ptr<DX12RootSignature> _rootsignature, LPCWSTR _name) {
		return boost::shared_ptr<DX12GraphicsPipeline>(
			DBG_NEW DX12GraphicsPipeline(
				mDevice, _vertexShader, _pixelShader, _vertexLayout,_primitive, _numrt,
				_rootsignature,_name)
		);
}


DXGI_FORMAT DX12GraphicsPipeline::mVertexLayoutFormatCorrespond[(unsigned char)DX12Config::VertexLayoutFormat::size] = {
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32_UINT
};

D3D12_INPUT_CLASSIFICATION DX12GraphicsPipeline::mVertexLayoutInputClassificationCorrespond[(unsigned char)DX12Config::VertexLayoutInputClassification::size] = {
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA
};

D3D12_PRIMITIVE_TOPOLOGY_TYPE DX12GraphicsPipeline::mPrimitiveTopologyTypeCorrespond[(unsigned char)DX12Config::PrimitiveTopologyType::size] = {
	D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
};

void DX12Pimple::SetGraphicsPipeline(boost::shared_ptr<DX12GraphicsPipeline> _pipeline)
{
	if (current_pipeline_ == _pipeline)return;
	_pipeline->SetGraphicsPipeline(mCmdList);
	current_pipeline_ = _pipeline;
}
