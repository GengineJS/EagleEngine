#include <modules/EagleWindow.hpp>

namespace eg {
#if defined(_WIN32)
	MSG msg;
	void EagleWindow::execute(ModuleStage)
	{
		_onReady(_winHandler);
	}
	bool EagleWindow::peekMessage()
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				return false;
			}
		}
		return true;
	}
	LRESULT EagleWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		EagleWindow::Get()->handleMessages(hWnd, uMsg, wParam, lParam);
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
	void EagleWindow::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg)
		{
		case WM_CLOSE:
			_onClose();
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			_onPaint();
			ValidateRect(_winHandler.window, NULL);
			break;
		case WM_KEYDOWN:
			//switch (wParam)
			//{
			//case KEY_P:
			//	// _paused = !_paused;
			//	break;
			//case KEY_F1:
			//	break;
			//case KEY_ESCAPE:
			//	PostQuitMessage(0);
			//	break;
			//}
			// keyPressed((uint32_t)wParam);
			_onKey(static_cast<uint32_t>(wParam), true);
			break;
		case WM_KEYUP:
			_onKey(static_cast<uint32_t>(wParam), false);
			break;
		case WM_LBUTTONDOWN:
			// mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			// mouseButton.left = true;
			_onMouse(0, true, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONDOWN:
			// mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			// mouseButton.right = true;
			_onMouse(1, true, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONDOWN:
			// mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			// mouseButton.middle = true;
			_onMouse(2, true, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			// mouseButton.left = false;
			_onMouse(0, false, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONUP:
			// mouseButton.right = false;
			_onMouse(1, false, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONUP:
			// mouseButton.middle = false;
			_onMouse(2, false, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MOUSEWHEEL:
		{
			short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			_onMouseWheel(wheelDelta);
			break;
		}
		case WM_MOUSEMOVE:
		{
			// handleMouseMove(LOWORD(lParam), HIWORD(lParam));
			_onMouseMove(LOWORD(lParam), HIWORD(lParam));
			break;
		}
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				_isMinimize = false;
				if ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED))
				{
					_destWidth = LOWORD(lParam);
					_destHeight = HIWORD(lParam);
					//- windowResize();
					_onSize(_destWidth, _destHeight);
				}
			}
			else {
				_isMinimize = true;
			}
			break;
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
			minMaxInfo->ptMinTrackSize.x = 64;
			minMaxInfo->ptMinTrackSize.y = 64;
			break;
		}
		case WM_ENTERSIZEMOVE:
			// _resizing = true;
			break;
		case WM_EXITSIZEMOVE:
			// _resizing = false;
			break;
		}
	}
	HWND EagleWindow::setup(HINSTANCE hinstance)
	{
		_winHandler.windowInstance = hinstance;
		const EngineInfo& info = EagleEngine::Get()->getEngineInfo();
		WNDCLASSEX wndClass;

		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hinstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = info.applicationName.c_str();
		wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

		if (!RegisterClassEx(&wndClass))
		{
			std::cout << "Could not register window class!\n";
			fflush(stdout);
			exit(1);
		}

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		bool fullscreen = info.fullscreen;
		if (fullscreen)
		{
			if ((info.width != (uint32_t)screenWidth) && (info.height != (uint32_t)screenHeight))
			{
				DEVMODE dmScreenSettings;
				memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
				dmScreenSettings.dmSize = sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth = info.width;
				dmScreenSettings.dmPelsHeight = info.height;
				dmScreenSettings.dmBitsPerPel = 32;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				{
					if (MessageBox(NULL, "Fullscreen Mode not supported!\n Switch to window mode?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
					{
						fullscreen = false;
					}
					else
					{
						return nullptr;
					}
				}
				screenWidth = info.width;
				screenHeight = info.height;
			}
		}

		DWORD dwExStyle;
		DWORD dwStyle;

		if (fullscreen)
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}

		RECT windowRect;
		windowRect.left = 0L;
		windowRect.top = 0L;
		windowRect.right = fullscreen ? (long)screenWidth : (long)info.width;
		windowRect.bottom = fullscreen ? (long)screenHeight : (long)info.height;

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

		std::string windowTitle = info.applicationName;
		_winHandler.window = CreateWindowEx(0,
			info.applicationName.c_str(),
			windowTitle.c_str(),
			dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			0,
			0,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			hinstance,
			NULL);

		if (!fullscreen)
		{
			// Center on screen
			uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
			uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
			SetWindowPos(_winHandler.window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}

		if (!_winHandler.window)
		{
			printf("Could not create window!\n");
			fflush(stdout);
			return nullptr;
		}

		ShowWindow(_winHandler.window, SW_SHOW);
		SetForegroundWindow(_winHandler.window);
		SetFocus(_winHandler.window);
		return _winHandler.window;
	}
#elif defined(USE_PLATFORM_ANDROID_KHR)
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
#else
#endif
}
