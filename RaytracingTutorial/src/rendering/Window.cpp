#include "Window.h"

namespace RT
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Window::getWindow()->msgProc(hwnd, msg, wParam, lParam);
	}

	Window* Window::mWindow = nullptr;
	Window* Window::getWindow() { return mWindow; }

	Window::Window(HINSTANCE instance): mWindowInst(instance)
	{
		assert(mWindow == nullptr);
		mWindow = this;
	}

	Window::~Window() {}

	bool Window::init()
	{
		if(!initWindow())
			return false;
		if(!initDirectX())
			return false;

		onResize();
		return true;
	}

	int Window::run()
	{
		MSG msg = { 0 };
		while(msg.message != WM_QUIT)
		{
			if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if(!mWindowPaused)
				{
					update();
					draw();
				}
			}
		}

		return 0;
	}

	LRESULT Window::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_ACTIVATE:
			mWindowPaused = LOWORD(wParam) == WA_INACTIVE;
			return 0;
		case WM_SIZE:
			settings.width = LOWORD(lParam);
			settings.height = HIWORD(lParam);

			if(wParam == SIZE_MINIMIZED)
			{
				mWindowPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if(wParam == SIZE_MAXIMIZED)
			{
				mWindowPaused = false;
				mMinimized = false;
				mMaximized = true;
				onResize();
			}
			else if(wParam == SIZE_RESTORED)
			{
				if(mMinimized)
				{
					mWindowPaused = false;
					mMinimized = false;
					onResize();
				}
				else if(mMaximized)
				{
					mWindowPaused = false;
					mMaximized = false;
					onResize();
				}
				else if(!mResizing)
					onResize();
			}
			return 0;
		case WM_ENTERSIZEMOVE:
			mWindowPaused = true;
			mResizing = true;
			return 0;
		case WM_EXITSIZEMOVE:
			mWindowPaused = false;
			mResizing = false;
			onResize();
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
		case WM_GETMINMAXINFO:
			((MINMAXINFO*) lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*) lParam)->ptMinTrackSize.y = 200;
			return 0;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Window::onResize() {}

	bool Window::initWindow()
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = mWindowInst;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"MainWnd";

		if(!RegisterClass(&wc))
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
			return false;
		}

		RECT R = { 0, 0, static_cast<LONG>(settings.width), static_cast<LONG>(settings.height) };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		mMainWin = CreateWindow(L"MainWnd", settings.title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mWindowInst, 0);
		if(!mMainWin)
		{
			MessageBox(0, L"CreateWindow Failed.", 0, 0);
			return false;
		}

		ShowWindow(mMainWin, SW_SHOW);
		UpdateWindow(mMainWin);
		return true;
	}

	bool Window::initDirectX()
	{
		return true;
	}

	void Window::getDisplayMode() {}

	void Window::createCmds() {}
	void Window::createSwapChain() {}
	void Window::flushCommandQueue() {}
};