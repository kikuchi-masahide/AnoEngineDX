#include "window.h"
#include "Game.h"
#include "Scene1.h"

LRESULT WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#ifdef _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif // _DEBUG
	Log::OutputTrivial("program entry");
	Game* game = new Game;
	if (!game->Initialize())return -1;
	game->AddWindow((WNDPROC)WindowProcedure, TEXT("WndClass"), 1024, 768, TEXT("Title here"),114514);
	auto windowptr = game->GetWindow(114514);
	auto mTextureDescHeap = game->mdx12.CreateDescriptorHeap(DX12Config::DescriptorHeapType::SRV, DX12Config::DescriptorHeapShaderVisibility::SHADER_VISIBLE, 1);
	game->mTexManager.LoadTexture(L"textest.png", mTextureDescHeap, 0, 0);
	game->mShaderManager.LoadShader(L"BasicVertexShader.hlsl", DX12Config::ShaderType::VERTEX, 0);
	game->mShaderManager.LoadShader(L"BasicPixelShader.hlsl", DX12Config::ShaderType::PIXEL, 1);
	game->mShaderManager.LoadShader(L"Layer2DVertexShader.hlsl", DX12Config::ShaderType::VERTEX, 2);
	game->mShaderManager.LoadShader(L"Layer2DPixelShader.hlsl", DX12Config::ShaderType::PIXEL, 3);
	game->ChangeScene<Scene1>();
	game->RunLoop();
	game->Shutdown();

	delete game;
	return 0;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}