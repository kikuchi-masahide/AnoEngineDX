//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "RootSignature.h"

#include "Log.h"

DX12::RootSignature::RootSignature(int root_param_num)
	:serialized_(false)
{
	root_params_.reserve(root_param_num);
}

void DX12::RootSignature::Serialize(ComPtr<ID3D12Device> device)
{
	assert(!serialized_);
	serialized_ = true;
	//HACK:あとからルートパラメタやサンプラーを追加する
	//RootParameterの設定
	D3D12_ROOT_PARAMETER* params = nullptr;
	if (root_params_.size() > 0) {
		params = DBG_NEW D3D12_ROOT_PARAMETER[root_params_.size()];
		for (int n = 0; n < root_params_.size(); n++) {
			params[n] = root_params_[n];
		}
	}
	D3D12_ROOT_SIGNATURE_DESC desc = CD3DX12_ROOT_SIGNATURE_DESC(
		root_params_.size(), params);
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ComPtr<ID3DBlob> root_sig_blob, error_blob;
	auto result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&root_sig_blob, &error_blob);
	for (int n = 0; n < root_params_.size(); n++) {
		if (params[n].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE) {
			delete[] params[n].DescriptorTable.pDescriptorRanges;
		}
	}
	if (params) {
		delete[] params;
	}
	if (FAILED(result)) {
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());
		std::copy_n((char*)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
		Log::OutputCritical(errstr);
		throw 0;
	}
	result = device->CreateRootSignature(
		0, root_sig_blob->GetBufferPointer(), root_sig_blob->GetBufferSize(),
		IID_PPV_ARGS(root_signature_.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		Log::OutputCritical("RootSignature's initialization failed\n");
		throw 0;
	}
}

void DX12::RootSignature::AddRootParameterAsDescriptorTable(std::vector<DescriptorRange>& ranges, D3D12_SHADER_VISIBILITY vis)
{
	assert(!serialized_);
	root_params_.emplace_back();
	auto& param = root_params_.back();
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(vis);
	D3D12_DESCRIPTOR_RANGE* range_desc = DBG_NEW D3D12_DESCRIPTOR_RANGE[ranges.size()];
	for (int n = 0; n < ranges.size(); n++) {
		DescriptorRange& range = ranges[n];
		D3D12_DESCRIPTOR_RANGE& desc = range_desc[n];
		desc.RangeType = range.range_type_;
		desc.NumDescriptors = range.num_descriptors_;
		desc.BaseShaderRegister = range.base_shader_register_;
		desc.OffsetInDescriptorsFromTableStart = (range.base_heap_register_ != -1 ? range.base_heap_register_ : D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
		desc.RegisterSpace = 0;
	}
	param.DescriptorTable.pDescriptorRanges = range_desc;
	param.DescriptorTable.NumDescriptorRanges = ranges.size();
}

void DX12::RootSignature::AddRootParameterAsCBV(UINT shader_register, D3D12_SHADER_VISIBILITY vis)
{
	assert(!serialized_);
	root_params_.emplace_back();
	auto& param = root_params_.back();
	param.InitAsConstantBufferView(shader_register, 0, static_cast<D3D12_SHADER_VISIBILITY>(vis));
}

void DX12::RootSignature::AddRootParameterAsSRV(UINT shader_register, D3D12_SHADER_VISIBILITY vis)
{
	assert(!serialized_);
	root_params_.emplace_back();
	auto& param = root_params_.back();
	param.InitAsShaderResourceView(shader_register, 0, static_cast<D3D12_SHADER_VISIBILITY>(vis));
}

void DX12::RootSignature::AddRootParameterAsConstant(UINT shader_register, SIZE_T const_size, D3D12_SHADER_VISIBILITY vis)
{
	assert(!serialized_);
	root_params_.emplace_back();
	auto& param = root_params_.back();
	int reg_num = static_cast<int>(const_size + 3) >> 2;
	param.InitAsConstants(reg_num, shader_register, 0, static_cast<D3D12_SHADER_VISIBILITY>(vis));
}

ID3D12RootSignature* DX12::RootSignature::GetRawPtr() const
{
	return root_signature_.Get();
}

