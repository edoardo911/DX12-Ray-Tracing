#pragma once

#include "../utils/header.h"

namespace RT
{
	class Window
	{
	public:
		static Window* getWindow();

		inline HINSTANCE windowInst() const { return mWindowInst; }
		inline HWND mainWin() const { return mMainWin; }
		inline float aspectRatio() const { return static_cast<float>(settings.width) / settings.height; }

		virtual ~Window();
		virtual bool init();
		int run();

		virtual LRESULT msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		Window(HINSTANCE instance);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void onResize();

		bool initWindow();
		bool initDirectX();

		void getDisplayMode();

		void createCmds();
		void createSwapChain();
		void flushCommandQueue();

		//get current back buffer

		//variables
		static Window* mWindow;

		//dx12 variables

		HINSTANCE mWindowInst = nullptr;
		
		int currBackBuffer = 0;

		UINT mRtvDescriptorSize = 0;
		UINT mDsvDescriptorSize = 0;
		UINT mCbvSrvUavDescriptorSize = 0;

		HWND mMainWin = nullptr;

		//display mode

		bool mWindowPaused = false;
		bool mMinimized = false;
		bool mMaximized = false;
		bool mResizing = false;

		settings_struct settings;
	};
};