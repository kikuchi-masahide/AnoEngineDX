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
	game->AddWindow((WNDPROC)WindowProcedure, TEXT("WndClass"), 1024, 768, TEXT("Title here"));
	auto windowptr = game->GetWindow(0);
	boost::shared_ptr<DX12DescriptorHeap> swapchaindescheap
		= game->mdx12.CreateDescriptorHeap(DX12Config::DescriptorHeapType::RTV, DX12Config::DescriptorHeapShaderVisibility::NONE, 2);
	game->mdx12.CreateSwapChain(windowptr->GetWindowHandle(), 1024, 768,swapchaindescheap);
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