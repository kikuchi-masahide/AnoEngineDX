#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"
#include "DX12RootParameter.h"

class DX12Device;
class DX12CmdList;

/// <summary>
/// ディスクリプタレンジ，ディスクリプタテーブルをまとめる構造体であるディスクリプタパラメータをコードするオブジェクト
/// </summary>
class DX12RootSignature final :public boost::noncopyable {
public:
	DX12RootSignature(ComPtr<ID3D12Device> _device, DX12RootParameter& _descheap_rootparam, bool _hasSampler);
	ComPtr<ID3D12RootSignature> mRootSignature;
	void SetRootSignature(ComPtr<ID3D12GraphicsCommandList> _list);
private:
	static D3D12_SHADER_VISIBILITY mShaderVisibilityCorrespond[(unsigned char)DX12Config::RootParameterShaderVisibility::size];
	static D3D12_DESCRIPTOR_RANGE_TYPE mDescRngTypeCorrespond[(unsigned char)DX12Config::DescriptorRangeType::size];
};