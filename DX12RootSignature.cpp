#include "DX12RootSignature.h"
#include "DX12Device.h"
#include "DX12CmdList.h"

DX12RootSignature::DX12RootSignature(DX12Device* _device)
{
	D3D12_ROOT_SIGNATURE_DESC RSDesc = {};
	RSDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3D12SerializeRootSignature(&RSDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		Log::OutputCritical(errstr);
		throw 0;
	}
	auto dev = _device->GetDevice();
	dev->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.ReleaseAndGetAddressOf())
	);
}

void DX12RootSignature::SetRootSignature(DX12CmdList* _list)
{
	_list->GetCmdList()->SetComputeRootSignature(mRootSignature.Get());
}
