#include "DX12RootSignature.h"
#include "DX12Device.h"
#include "DX12CmdList.h"

DX12RootSignature::DX12RootSignature(DX12Device* _device, DX12RootParameter& _rootparam)
{
	D3D12_ROOT_SIGNATURE_DESC RSDesc = {};
	
	RSDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12_DESCRIPTOR_RANGE* descTblRng = nullptr;
	//ルートパラメタが空でない場合
	if (_rootparam.mDescRanges.size() > 0) {
		//ルートパラメタを設定
		descTblRng = new D3D12_DESCRIPTOR_RANGE[_rootparam.mDescRanges.size()];
		for (int n = 0; n < _rootparam.mDescRanges.size(); n++) {
			descTblRng[n] = {};
			descTblRng[n].NumDescriptors = _rootparam.mDescRanges[n].mNumDescriptors;
			descTblRng[n].BaseShaderRegister = _rootparam.mDescRanges[n].mBaseShaderRegister;
			descTblRng[n].RangeType = mDescRngTypeCorrespond[(unsigned int)_rootparam.mDescRanges[n].mType];
			descTblRng[n].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		}

		D3D12_ROOT_PARAMETER rootparam = {};
		rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam.ShaderVisibility = mShaderVisibilityCorrespond[(unsigned int)_rootparam.mShaderVisibility];
		rootparam.DescriptorTable.pDescriptorRanges = descTblRng;
		rootparam.DescriptorTable.NumDescriptorRanges = _rootparam.mDescRanges.size();

		RSDesc.pParameters = &rootparam;
		RSDesc.NumParameters = 1;
	}
	//サンプラー設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

	RSDesc.pStaticSamplers = &samplerDesc;
	RSDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3D12SerializeRootSignature(&RSDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	if(descTblRng)delete[] descTblRng;
	if (FAILED(result)) {
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		Log::OutputCritical(errstr);
		throw 0;
	}

	auto dev = _device->GetDevice();
	result = dev->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		Log::OutputCritical("RootSignature's initialization failed\n");
		throw 0;
	}
}

void DX12RootSignature::SetRootSignature(DX12CmdList* _list)
{
	_list->GetCmdList()->SetGraphicsRootSignature(mRootSignature.Get());
}

D3D12_SHADER_VISIBILITY DX12RootSignature::mShaderVisibilityCorrespond[(unsigned char)DX12Config::ShaderVisibility::size] = {
	D3D12_SHADER_VISIBILITY_ALL,
	D3D12_SHADER_VISIBILITY_ALL
};

D3D12_DESCRIPTOR_RANGE_TYPE DX12RootSignature::mDescRngTypeCorrespond[(unsigned char)DX12Config::DescriptorHeapType::size] = {
	D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
	D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
	D3D12_DESCRIPTOR_RANGE_TYPE_CBV
};
