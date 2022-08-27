//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ShaderObject.h"

#include "Log.h"

DX12::ShaderObject::ShaderObject(LPCWSTR filename, ShaderType type)
{
	LPCSTR shader_type = "";
	switch (type) {
	case ShaderType::VertexShader:
		shader_type = "vs_5_0";
		break;
	case ShaderType::PixelShader:
		shader_type = "ps_5_0";
		break;
	}
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3DCompileFromFile(
		filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shader_type,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &blob_, &errorBlob
	);
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			//HACK:Log‚ÅLPCWSTR‚ðˆµ‚¦‚é‚æ‚¤‚É‚·‚é?
			Log::OutputCritical("shader file not found");
		}
		else {
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			Log::OutputCritical(errstr);
		}
		throw 0;
	}
}

const void* DX12::ShaderObject::GetBufferPointer()
{
	return blob_->GetBufferPointer();
}

SIZE_T DX12::ShaderObject::GetBufferSize()
{
	return blob_->GetBufferSize();
}
