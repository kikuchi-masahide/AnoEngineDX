#include "window.h"
#include "Game.h"

LRESULT WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#ifdef _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif // _DEBUG
	Log::OutputTrivial("program entry");
	Game game;
	if (!game.Initialize())return -1;
	game.AddWindow((WNDPROC)WindowProcedure, TEXT("WndClass"), 1024, 768, TEXT("Title here"));
	game.RunLoop();
	game.Shutdown();
	return 0;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}