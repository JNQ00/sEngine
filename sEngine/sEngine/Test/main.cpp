#include <Headers/sRenderer.h>
#include <windows.h>
#include "Tools/Log.hpp"

using namespace sRender;

bool quitApp;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		DestroyWindow(hWnd);
		quitApp = true;
		return true;
		break;
	case WM_NCCREATE:
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
		return true;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}

int main(int argc, char** argv)
{

	quitApp = false;

	//Create window
	MSG msg = { 0 };
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(0);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = "TEST";
	if (!RegisterClass(&wc))
		Log::e("RENDER") << "Could not register class: " << GetLastError();;

	HWND myHWND = CreateWindowEx(
		0,                              // Optional window styles.
		wc.lpszClassName,                     // Window class
		"TEST",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(0),  // Instance handle
		NULL        // Additional application data
		);

	if (myHWND == NULL)
	{
		Log::e("RENDER") << "Could not create window: " << GetLastError();
	}

	SetWindowText(myHWND, "TEST");
	ShowWindow(myHWND, SW_SHOWDEFAULT);


	//-------------------------------------------------------------
	//-----REAL USAGE HERE-----------------------------------------
	//-------------------------------------------------------------
	sRenderer renderer;

	sRenderContextCFG myCFG;
	myCFG.Debug = true;
	myCFG.Width = 800;
	myCFG.Height = 600;
	myCFG.Format = sTexFormat_RGBA8_Unorm;
	myCFG.SampleCount = 1;
	myCFG.PlatformData = (void*)myHWND;

	renderer.CreateRenderContext(myCFG);

	while (!quitApp)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	renderer.Destroy();
	
	return 0;
}