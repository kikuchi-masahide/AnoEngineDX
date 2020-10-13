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
	boost::shared_ptr<Game> game(new Game());
	if (!game->Initialize())return -1;
	game->AddWindow((WNDPROC)WindowProcedure, TEXT("WndClass"), 1024, 768, TEXT("Title here"));
	auto windowptr = game->GetWindow(0);
	game->mdx12.CreateSwapChain(windowptr->GetWindowHandle(), 1024, 768);
	boost::shared_ptr<Scene1> scene(new Scene1(game));
	scene->SetWeakThis(scene);
	scene->Set();
	game->ChangeScene(scene);
	game->RunLoop();
	game->Shutdown();
	return 0;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}