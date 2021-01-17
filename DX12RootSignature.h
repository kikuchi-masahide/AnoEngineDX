#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"
#include "DX12RootParameter.h"

class DX12Device;
class DX12CmdList;

class DX12RootSignature final :public boost::noncopyable {
public:
	DX12RootSignature(ComPtr<ID3D12Device> _device,DX12RootParameter& _rootparam);
	ComPtr<ID3D12RootSignature> mRootSignature;
	void SetRootSignature(ComPtr<ID3D12GraphicsCommandList> _list);
private:
	static D3D12_SHADER_VISIBILITY mShaderVisibilityCorrespond[(unsigned char)DX12Config::ShaderVisibility::size];
	static D3D12_DESCRIPTOR_RANGE_TYPE mDescRngTypeCorrespond[(unsigned char)DX12Config::DescriptorHeapType::size];
};