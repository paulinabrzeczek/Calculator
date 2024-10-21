#include "Main.h"
#include "Windows.h"

const char Calculate[] = "kalkulator";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX winClass;
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpszClassName = Calculate;

	if (!RegisterClassEx(&winClass))
	{
		MessageBox(NULL, "B³¹d rejestracji tego okna", "B³¹d", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}
	HWND hwnd;

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, Calculate, "Okno g³ówne", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 360, 240, NULL, NULL, hInstance, NULL);
	
	if (hwnd == NULL)
	{
		MessageBox(NULL, "B³¹d otwarcie okna", "B³¹d", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}