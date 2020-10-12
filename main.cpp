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
	int use = game.use_count();
	if (!game->Initialize())return -1;
	game->AddWindow((WNDPROC)WindowProcedure, TEXT("WndClass"), 1024, 768, TEXT("Title here"));
	boost::shared_ptr<Scene1> scene(new Scene1(game));
	use = game.use_count();
	Log::OutputTrivial(std::to_string(use).c_str());
	scene->SetWeakThis(scene);
	scene->Set();
	Log::OutputTrivial(std::to_string(game.use_count()).c_str());
	game->ChangeScene(scene);
	Log::OutputTrivial(std::to_string(game.use_count()).c_str());
	game->RunLoop();
	game->Shutdown();
	Log::OutputTrivial(std::to_string(game.use_count()).c_str());
	Log::OutputTrivial(std::to_string(scene.use_count()).c_str());
	return 0;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}