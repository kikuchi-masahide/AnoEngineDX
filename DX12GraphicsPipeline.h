#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"
#include "DX12VertexLayoutUnit.h"

class DX12Device;
class DX12ShaderObject;
class DX12RootSignature;

class DX12GraphicsPipeline final :public boost::noncopyable {
public:
	/// <summary>
	/// グラフィックスパイプラインの作成
	/// </summary>
	/// <param name="_vertexShader">頂点シェーダ</param>
	/// <param name="_pixelShader">ピクセルシェーダ</param>
	/// <param name="_vertexLayout">頂点レイアウト</param>
	/// <param name="_primitive">プリミティブ型</param>
	/// <param name="_numrt">レンダーターゲット数</param>
	DX12GraphicsPipeline(DX12Device* _device, boost::shared_ptr<DX12ShaderObject> _vertexShader,
		boost::shared_ptr<DX12ShaderObject> _pixelShader, DX12VertexLayout& _vertexLayout,
		DX12Config::PrimitiveTopologyType _primitive, UINT _numrt,
		boost::shared_ptr<DX12RootSignature> _rootsignature);
private:
	ComPtr<ID3D12PipelineState> mState;
	static DXGI_FORMAT mVertexLayoutFormatCorrespond[(unsigned char)DX12Config::VertexLayoutFormat::size];
	static D3D12_INPUT_CLASSIFICATION mVertexLayoutInputClassificationCorrespond[(unsigned char)DX12Config::VertexLayoutInputClassification::size];
	static D3D12_PRIMITIVE_TOPOLOGY_TYPE mPrimitiveTopologyTypeCorrespond[(unsigned char)DX12Config::PrimitiveTopologyType::size];
};