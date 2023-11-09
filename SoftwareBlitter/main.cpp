
#include <Windows.h>

#if defined(DEBUG) | defined(_DEBUG)
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#	include <memory>
#endif

#include "Library/Timer.h"
#include "Library/Error.h"
#include "Game.h"
#include "Settings.h"
#include "SoftwareRenderer.h"

bool running = false;
HWND gameWindow = 0;

/*****************************************************************************************/
LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_KEYMENU:
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;
		}

		case WM_PAINT:
			ValidateRect(hWindow, NULL);
			return 0;

		case WM_TIMER:
			switch (wParam)
			{
				case 500: // refresh
					break;
			}
			return 0;

		case WM_LBUTTONDOWN:
			return 0;

		case WM_DESTROY:
			DestroyWindow(hWindow);
			running = false;
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWindow, msg, wParam, lParam);
}

/*****************************************************************************************/
bool CreateGameWindow(HINSTANCE instance, const char* windowTitle, bool fullscreen, HWND& result, int cmdShow)
{
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = instance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = windowTitle;
	wndclass.hIconSm = NULL;

	if (!RegisterClassEx(&wndclass))
	{
		return false;
	}

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = Settings::ScreenWidth;
	rc.bottom = Settings::ScreenHeight;

	if (fullscreen)
	{
		result = CreateWindowEx(0, windowTitle, windowTitle, WS_POPUP, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	}
	else
	{
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		result = CreateWindowEx(0, windowTitle, windowTitle, WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	}

	ShowWindow(result, cmdShow);
	UpdateWindow(result);
	InvalidateRect(result, NULL, TRUE);

	return true;
}

/*****************************************************************************************/
void RunDemo()
{
	SoftwareRenderer::Create(gameWindow);
	Timer* timer = new Timer();
	Game* game = new Game();

	running = true;
	MSG msg;

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			BOOL bGetResult = GetMessage(&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		timer->Update();
		game->Update(timer->GetElapsedFrameTime());

		SoftwareRenderer::GetCurrentInstance()->Reset();
		game->Render();
		SoftwareRenderer::GetCurrentInstance()->Present();

		if (msg.message == WM_QUIT)
		{
			running = false;
		}
	}

	delete game;
	delete timer;
	SoftwareRenderer::Destroy();
}

/*****************************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	FILE* log = NULL;
	fopen_s(&log, "log.txt", "a+");

	HWND window = 0;
	if (CreateGameWindow(hInstance, "Sprite Blitter", false, gameWindow, iCmdShow))
	{
		RunDemo();
	}
	else
	{
		DisplayGameError("Failed to create game window");
	}

	fclose(log);
	log = NULL;
}