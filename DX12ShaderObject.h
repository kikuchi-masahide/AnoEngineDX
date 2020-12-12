#pragma once

#include "DX12CommonInclude.h"
#include "DX12ConfigEnums.h"

class DX12ShaderObject final :public boost::noncopyable {
public:
	DX12ShaderObject(LPCWSTR _filename,DX12Config::ShaderType _shaderType);
	const void* GetBufferPointer();
	SIZE_T GetBufferSize();
private:
	ComPtr<ID3DBlob> mBlob;
	static LPCSTR mShaderTypeCorrespond[(unsigned char)DX12Config::ShaderType::size];
};