#include "DX12.h"
#include "DX12Pimple.h"

DX12::DX12()
	:mPimple(new DX12Pimple) {}

void DX12::Initialize()
{
	mPimple->Initialize();
}

void DX12::CleanUp()
{
	mPimple->CleanUp();
	delete mPimple;
}