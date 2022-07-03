#include "DX12ShaderObject.h"
#include "DX12Pimple.h"
#include "../Log.h"

DX12ShaderObject::DX12ShaderObject(LPCWSTR _filename, DX12Config::ShaderType _shaderType)
{
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3DCompileFromFile(
		_filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
		mShaderTypeCorrespond[(unsigned char)_shaderType],
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		&mBlob, &errorBlob
	);
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			Log::OutputCritical("shader file not found");
		}
		else {
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(),errstr.begin());
			Log::OutputCritical(errstr);
		}
		throw 0;
	}
}

const void* DX12ShaderObject::GetBufferPointer()
{
	return mBlob->GetBufferPointer();
}

SIZE_T DX12ShaderObject::GetBufferSize()
{
	return mBlob->GetBufferSize();
}

LPCSTR DX12ShaderObject::mShaderTypeCorrespond[(unsigned char)DX12Config::ShaderType::size] = {
	"vs_5_0","ps_5_0"
};

boost::shared_ptr<DX12ShaderObject> DX12Pimple::LoadShader(LPCWSTR _filename, DX12Config::ShaderType _shaderType)
{
	return boost::shared_ptr<DX12ShaderObject>(new DX12ShaderObject(
		_filename, _shaderType
	));
}
