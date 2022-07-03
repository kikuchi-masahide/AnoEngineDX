#include "Game.h"

LRESULT WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#ifdef _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif // _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Game* game = new Game();
	game->Initialize();
	game->AddWindow(WindowProcedure, TEXT("WndClass"), 900, 900, TEXT("AnoEngineDX"), 0);
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