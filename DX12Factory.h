#pragma once

#include "DX12CommonInclude.h"

class DX12Factory :public boost::noncopyable
{
public:
	DX12Factory();
	void Initialize();
	ComPtr<IDXGIFactory6> GetFactory();
	void CleanUp();
private:
	ComPtr<IDXGIFactory6> mFactory;
};