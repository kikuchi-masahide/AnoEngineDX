#include "DX12RootSignature.h"
#include "DX12Pimple.h"
#include "../Log.h"

DX12RootSignature::DX12RootSignature(ComPtr<ID3D12Device> _device, DX12RootParameter& _descheap_rootparam, bool _hasSampler)
{
	D3D12_ROOT_SIGNATURE_DESC RSDesc = {};

	RSDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//ルートパラメータの構造体
	D3D12_ROOT_PARAMETER d3drootparameter = {};
	//同じく後からdelete[]しなければならない，ディスクリプタレンジ構造体の配列の集合
	std::vector<D3D12_DESCRIPTOR_RANGE*> ranges_array_to_delete;
	if (_descheap_rootparam.mDescRanges.size() > 0) {
		DX12RootParameter& param_from = _descheap_rootparam;
		//今から設定するルートパラメータの構造体
		D3D12_ROOT_PARAMETER& param_to = d3drootparameter;
		param_to = {};
		//このルートパラメータをディスクリプタテーブルに紐づける
		param_to.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//このルートパラメタの持つレンジの数
		param_to.DescriptorTable.NumDescriptorRanges = param_from.mDescRanges.size();
		//シェーダからの可視性
		param_to.ShaderVisibility = mShaderVisibilityCorrespond[(unsigned char)param_from.mShaderVisibility];
		//レンジの数が0以上の場合，レンジの配列を作る
		if (param_from.mDescRanges.size() > 0)
		{
			D3D12_DESCRIPTOR_RANGE* d3d_descriptor_range = new D3D12_DESCRIPTOR_RANGE[param_from.mDescRanges.size()];
			ranges_array_to_delete.push_back(d3d_descriptor_range);
			for (unsigned int range_ind = 0; range_ind < param_from.mDescRanges.size(); range_ind++)
			{
				DX12DescriptorRange& range_from = param_from.mDescRanges[range_ind];
				//今から設定するレンジの構造体
				D3D12_DESCRIPTOR_RANGE& range_to = d3d_descriptor_range[range_ind];
				//レンジのもつビューの種類
				range_to.RangeType = mDescRngTypeCorrespond[(unsigned char)range_from.mType];
				//ディスクリプタの数
				range_to.NumDescriptors = range_from.mNumDescriptors;
				//シェーダレジスタのどこからか
				range_to.BaseShaderRegister = range_from.mBaseShaderRegister;
				//ディスクリプタヒープのどこからか
				//HACK:とりあえずはoffset_appendでいいが，
				range_to.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//range_from.mBaseHeapRegister;
				range_to.RegisterSpace = 0;
			}
			param_to.DescriptorTable.pDescriptorRanges = d3d_descriptor_range;
		}
		//ルートパラメータ配列を指定
		RSDesc.pParameters = &d3drootparameter;
		//ルートパラメータの数
		RSDesc.NumParameters = 1;
	}

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	if (_hasSampler) {
		//サンプラー設定
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

		RSDesc.pStaticSamplers = &samplerDesc;
		RSDesc.NumStaticSamplers = 1;
	}
	else {
		RSDesc.pStaticSamplers = nullptr;
		RSDesc.NumStaticSamplers = 0;
	}

	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3D12SerializeRootSignature(&RSDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	for (auto arr : ranges_array_to_delete)
	{
		delete[] arr;
	}
	if (FAILED(result)) {
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		Log::OutputCritical(errstr);
		throw 0;
	}

	result = _device->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		Log::OutputCritical("RootSignature's initialization failed\n");
		throw 0;
	}
}

void DX12RootSignature::SetRootSignature(ComPtr<ID3D12GraphicsCommandList> _list)
{
	_list->SetGraphicsRootSignature(mRootSignature.Get());
}

D3D12_SHADER_VISIBILITY DX12RootSignature::mShaderVisibilityCorrespond[(unsigned char)DX12Config::RootParameterShaderVisibility::size] = {
	D3D12_SHADER_VISIBILITY_ALL,
	D3D12_SHADER_VISIBILITY_VERTEX,
	D3D12_SHADER_VISIBILITY_PIXEL
};

D3D12_DESCRIPTOR_RANGE_TYPE DX12RootSignature::mDescRngTypeCorrespond[(unsigned char)DX12Config::DescriptorRangeType::size] = {
	D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	D3D12_DESCRIPTOR_RANGE_TYPE_UAV
};

boost::shared_ptr<DX12RootSignature> DX12Pimple::CreateRootSignature(DX12RootParameter& _descheap_rootparam, bool _hasSampler)
{
	return boost::shared_ptr<DX12RootSignature>(new DX12RootSignature(
		mDevice, _descheap_rootparam, _hasSampler
	));
}

void DX12Pimple::SetRootSignature(boost::shared_ptr<DX12RootSignature> _root)
{
	if (current_rootsign_ == _root)return;
	_root->SetRootSignature(mCmdList);
	current_rootsign_ = _root;
}