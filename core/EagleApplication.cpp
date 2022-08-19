#include <EagleApplication.h>
#include <iostream>

#if (defined(USE_PLATFORM_MACOS_MVK) && defined(EXAMPLE_XCODE_GENERATED))
#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include <QuartzCore/CAMetalLayer.h>
#include <CoreVideo/CVDisplayLink.h>
#endif

void EagleApplication::keyPressed(uint32_t)
{
}
void EagleApplication::mouseMoved(double x, double y, bool& handled) {}
void EagleApplication::initialize()
{
	_renderer->onApplicationInit(shared_from_this());
	_renderer->onStart();
	_prepared = true;
	_isInit = true;
}
void EagleApplication::appExit()
{
	_prepared = false;
	_renderer->onApplicationExit(shared_from_this());
}
void EagleApplication::appPause()
{
	_renderer->onApplicationPause();
	_handler.focused = false;
	_prepared = false;
}
void EagleApplication::appFocus()
{
    _renderer->onApplicationFocus();
	_handler.focused = true;
	_prepared = true;
}
void EagleApplication::renderLoop()
{
	/*if (benchmark.active) {
		benchmark.run([=] { render(); }, vulkanDevice->properties);
		vkDeviceWaitIdle(device);
		if (benchmark.filename != "") {
			benchmark.saveResults();
		}
		return;
	}*/
	_lastTimestamp = std::chrono::high_resolution_clock::now();
#if defined(_WIN32)
	MSG msg;
	bool quitMessageReceived = false;
	while (!quitMessageReceived) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				quitMessageReceived = true;
				break;
			}
		}
		if (_prepared && !IsIconic(_handler.window)) {
			nextFrame();
		}
	}
#elif defined(USE_PLATFORM_ANDROID_KHR)
	while (1)
	{
		int ident;
		int events;
		struct android_poll_source* source;
		bool destroy = false;

		// _handler.focused = true;

		while ((ident = ALooper_pollAll(_handler.focused ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
		{
			if (source != NULL)
			{
				source->process(_handler.androidApp, source);
			}

			if (_handler.androidApp->destroyRequested != 0)
			{
				LOGD("Android app destroy requested");
				destroy = true;
				break;
			}
		}

		// App destruction requested
		// Exit loop, example will be destroyed in application main
		if (destroy)
		{
			ANativeActivity_finish(_handler.androidApp->activity);
			break;
		}

		// Render frame
		if (_prepared)
		{
			auto tStart = std::chrono::high_resolution_clock::now();
            _renderer->onUpdate();
            _renderer->onRender();
			_frameCounter++;
			auto tEnd = std::chrono::high_resolution_clock::now();
			auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
			_frameTimer = tDiff / 1000.0f;
			// camera.update(_frameTimer);
			// Convert to clamped timer value
			if (!_paused)
			{
				_timer += _timerSpeed * _frameTimer;
				if (_timer > 1.0f)
				{
					_timer -= 1.0f;
				}
			}
			float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - _lastTimestamp).count();
			if (fpsTimer > 1000.0f)
			{
				_lastFPS = (float)_frameCounter * (1000.0f / fpsTimer);
				_frameCounter = 0;
				_lastTimestamp = tEnd;
			}

			// TODO: Cap UI overlay update rates/only issue when update requested
			// updateOverlay();

			// bool updateView = false;

			// Check touch state (for movement)
			if (_handler.touchDown) {
				_handler.touchTimer += _frameTimer;
			}
			if (_handler.touchTimer >= 1.0) {
				/*camera.keys.up = true;
				viewChanged();*/
			}

			// Check gamepad state
			// const float deadZone = 0.0015f;
			// todo : check if gamepad is present
			// todo : time based and relative axis positions
			/*if (camera.type != Camera::CameraType::firstperson)
			{
				// Rotate
				if (std::abs(gamePadState.axisLeft.x) > deadZone)
				{
					camera.rotate(glm::vec3(0.0f, gamePadState.axisLeft.x * 0.5f, 0.0f));
					updateView = true;
				}
				if (std::abs(gamePadState.axisLeft.y) > deadZone)
				{
					camera.rotate(glm::vec3(gamePadState.axisLeft.y * 0.5f, 0.0f, 0.0f));
					updateView = true;
				}
				// Zoom
				if (std::abs(gamePadState.axisRight.y) > deadZone)
				{
					camera.translate(glm::vec3(0.0f, 0.0f, gamePadState.axisRight.y * 0.01f));
					updateView = true;
				}
				if (updateView)
				{
					viewChanged();
				}
			}
			else
			{
				updateView = camera.updatePad(gamePadState.axisLeft, gamePadState.axisRight, _frameTimer);
				if (updateView)
				{
					viewChanged();
				}
			}*/
		}
	}
#elif (defined(USE_PLATFORM_MACOS_MVK) && defined(EXAMPLE_XCODE_GENERATED))
	[NSApp run];
#endif
}
inline void EagleApplication::nextFrame()
{
	auto tStart = std::chrono::high_resolution_clock::now();
	_renderer->onUpdate();
	_renderer->onRender();
	_frameCounter++;
	auto tEnd = std::chrono::high_resolution_clock::now();
	auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
	_frameTimer = (float)tDiff / 1000.0f;
	// Convert to clamped timer value
	if (!_paused)
	{
		_timer += _timerSpeed * _frameTimer;
		if (_timer > 1.0)
		{
			_timer -= 1.0f;
		}
	}
	float fpsTimer = (float)(std::chrono::duration<double, std::milli>(tEnd - _lastTimestamp).count());
	if (fpsTimer > 1000.0f)
	{
		_lastFPS = static_cast<uint32_t>((float)_frameCounter * (1000.0f / fpsTimer));
#if defined(_WIN32)
		if (!_renderer->getContext()->getContextInfo().guiOverlay) {
			std::string windowTitle = getWindowTitle();
			SetWindowText(_handler.window, windowTitle.c_str());
		}
#endif
		_frameCounter = 0;
		_lastTimestamp = tEnd;
	}
}
void EagleApplication::windowResize() {
	if (!_prepared)
	{
		return;
	}
	_prepared = false;
	_renderer->onResize(_destWidth, _destHeight);
	_prepared = true;
}
void EagleApplication::handleMouseMove(int32_t x, int32_t y)
{
	int32_t dx = (int32_t)mousePos.x - x;
	int32_t dy = (int32_t)mousePos.y - y;

	bool handled = false;
	
	if (_renderer->getContext()->getContextInfo().guiOverlay) {
		ImGuiIO& io = ImGui::GetIO();
		handled = io.WantCaptureMouse;
	}
	mouseMoved((float)x, (float)y, handled);

	if (handled) {
		mousePos = glm::vec2((float)x, (float)y);
		return;
	}

	if (mouseButton.left) {
	}
	if (mouseButton.right) {
	}
	if (mouseButton.middle) {
	}
	mousePos = glm::vec2((float)x, (float)y);
}

EagleApplication::EagleApplication(std::shared_ptr<EagleRenderer> renderer)
{
	_renderer = renderer;
}

#if defined(_WIN32)
// Win32 : Sets up a console window and redirects standard output to it
void EagleApplication::setupConsole(std::string title)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w+", stdout);
	freopen_s(&stream, "CONOUT$", "w+", stderr);
	SetConsoleTitle(TEXT(title.c_str()));
}

void EagleApplication::setupDPIAwareness()
{
	typedef HRESULT* (__stdcall* SetProcessDpiAwarenessFunc)(PROCESS_DPI_AWARENESS);

	HMODULE shCore = LoadLibraryA("Shcore.dll");
	if (shCore)
	{
		SetProcessDpiAwarenessFunc setProcessDpiAwareness =
			(SetProcessDpiAwarenessFunc)GetProcAddress(shCore, "SetProcessDpiAwareness");

		if (setProcessDpiAwareness != nullptr)
		{
			setProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		}

		FreeLibrary(shCore);
	}
}

HWND EagleApplication::setupWindow(HINSTANCE hinstance, WNDPROC wndproc)
{
	_handler.windowInstance = hinstance;
	const ContextInfo& info = _renderer->getContext()->getContextInfo();
	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndproc;
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

	std::string windowTitle = getWindowTitle();
	_handler.window = CreateWindowEx(0,
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
		SetWindowPos(_handler.window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	if (!_handler.window)
	{
		printf("Could not create window!\n");
		fflush(stdout);
		return nullptr;
	}

	ShowWindow(_handler.window, SW_SHOW);
	SetForegroundWindow(_handler.window);
	SetFocus(_handler.window);
	return _handler.window;
}

void EagleApplication::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		appExit();
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(_handler.window, NULL);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case KEY_P:
			_paused = !_paused;
			break;
		case KEY_F1:
			break;
		case KEY_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		keyPressed((uint32_t)wParam);
		break;
	case WM_KEYUP:
		break;
	case WM_LBUTTONDOWN:
		mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		mouseButton.left = true;
		break;
	case WM_RBUTTONDOWN:
		mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		mouseButton.right = true;
		break;
	case WM_MBUTTONDOWN:
		mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		mouseButton.middle = true;
		break;
	case WM_LBUTTONUP:
		mouseButton.left = false;
		break;
	case WM_RBUTTONUP:
		mouseButton.right = false;
		break;
	case WM_MBUTTONUP:
		mouseButton.middle = false;
		break;
	case WM_MOUSEWHEEL:
	{
		short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		handleMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_SIZE:
		if ((_prepared) && (wParam != SIZE_MINIMIZED))
		{
			if ((_resizing) || ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)))
			{
				_destWidth = LOWORD(lParam);
				_destHeight = HIWORD(lParam);
				windowResize();
			}
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
		_resizing = true;
		break;
	case WM_EXITSIZEMOVE:
		_resizing = false;
		break;
	}
}
#elif defined(USE_PLATFORM_ANDROID_KHR)
int32_t EagleApplication::handleAppInput(struct android_app* app, AInputEvent* event)
{

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		int32_t eventSource = AInputEvent_getSource(event);
		switch (eventSource) {
		case AINPUT_SOURCE_JOYSTICK: {
			// Left thumbstick
			/*eagleApp->gamePadState.axisLeft.x = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_X, 0);
			eagleApp->gamePadState.axisLeft.y = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Y, 0);
			// Right thumbstick
			eagleApp->gamePadState.axisRight.x = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Z, 0);
			eagleApp->gamePadState.axisRight.y = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RZ, 0);*/
			break;
		}

		case AINPUT_SOURCE_TOUCHSCREEN: {
			int32_t action = AMotionEvent_getAction(event);

			switch (action) {
			case AMOTION_EVENT_ACTION_UP: {
				/*eagleApp->lastTapTime = AMotionEvent_getEventTime(event);
				eagleApp->touchPos.x = AMotionEvent_getX(event, 0);
				eagleApp->touchPos.y = AMotionEvent_getY(event, 0);
				eagleApp->touchTimer = 0.0;
				eagleApp->touchDown = false;
				eagleApp->camera.keys.up = false;

				// Detect single tap
				int64_t eventTime = AMotionEvent_getEventTime(event);
				int64_t downTime = AMotionEvent_getDownTime(event);
				if (eventTime - downTime <= vks::android::TAP_TIMEOUT) {
					float deadZone = (160.f / vks::android::screenDensity) * vks::android::TAP_SLOP * vks::android::TAP_SLOP;
					float x = AMotionEvent_getX(event, 0) - eagleApp->touchPos.x;
					float y = AMotionEvent_getY(event, 0) - eagleApp->touchPos.y;
					if ((x * x + y * y) < deadZone) {
						eagleApp->mouseButtons.left = true;
					}
				};*/

				return 1;
				break;
			}
			case AMOTION_EVENT_ACTION_DOWN: {
				// Detect double tap
				/*int64_t eventTime = AMotionEvent_getEventTime(event);
				if (eventTime - eagleApp->lastTapTime <= vks::android::DOUBLE_TAP_TIMEOUT) {
					float deadZone = (160.f / vks::android::screenDensity) * vks::android::DOUBLE_TAP_SLOP * vks::android::DOUBLE_TAP_SLOP;
					float x = AMotionEvent_getX(event, 0) - eagleApp->touchPos.x;
					float y = AMotionEvent_getY(event, 0) - eagleApp->touchPos.y;
					if ((x * x + y * y) < deadZone) {
						eagleApp->keyPressed(TOUCH_DOUBLE_TAP);
						eagleApp->touchDown = false;
					}
				}
				else {
					eagleApp->touchDown = true;
				}
				eagleApp->touchPos.x = AMotionEvent_getX(event, 0);
				eagleApp->touchPos.y = AMotionEvent_getY(event, 0);
				eagleApp->mousePos.x = AMotionEvent_getX(event, 0);
				eagleApp->mousePos.y = AMotionEvent_getY(event, 0);*/
				break;
			}
			case AMOTION_EVENT_ACTION_MOVE: {
				bool handled = false;
				/*if (eagleApp->settings.overlay) {
					ImGuiIO& io = ImGui::GetIO();
					handled = io.WantCaptureMouse;
				}
				if (!handled) {
					int32_t eventX = AMotionEvent_getX(event, 0);
					int32_t eventY = AMotionEvent_getY(event, 0);

					float deltaX = (float)(eagleApp->touchPos.y - eventY) * eagleApp->camera.rotationSpeed * 0.5f;
					float deltaY = (float)(eagleApp->touchPos.x - eventX) * eagleApp->camera.rotationSpeed * 0.5f;

					eagleApp->camera.rotate(glm::vec3(deltaX, 0.0f, 0.0f));
					eagleApp->camera.rotate(glm::vec3(0.0f, -deltaY, 0.0f));

					eagleApp->viewChanged();

					eagleApp->touchPos.x = eventX;
					eagleApp->touchPos.y = eventY;
				}*/
				break;
			}
			default:
				return 1;
				break;
			}
		}

									  return 1;
		}
	}

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
	{
		int32_t keyCode = AKeyEvent_getKeyCode((const AInputEvent*)event);
		int32_t action = AKeyEvent_getAction((const AInputEvent*)event);
		int32_t button = 0;

		if (action == AKEY_EVENT_ACTION_UP)
			return 0;

		switch (keyCode)
		{
		case AKEYCODE_BUTTON_A:
			// eagleApp->keyPressed(GAMEPAD_BUTTON_A);
			break;
		case AKEYCODE_BUTTON_B:
			// eagleApp->keyPressed(GAMEPAD_BUTTON_B);
			break;
		case AKEYCODE_BUTTON_X:
			// eagleApp->keyPressed(GAMEPAD_BUTTON_X);
			break;
		case AKEYCODE_BUTTON_Y:
			// eagleApp->keyPressed(GAMEPAD_BUTTON_Y);
			break;
		case AKEYCODE_BUTTON_L1:
			// eagleApp->keyPressed(GAMEPAD_BUTTON_L1);
			break;
		case AKEYCODE_BUTTON_R1:
			// eagleApp->keyPressed(GAMEPAD_BUTTON_R1);
			break;
		case AKEYCODE_BUTTON_START:
			// eagleApp->paused = !eagleApp->paused;
			break;
		};

		LOGD("Button %d pressed", keyCode);
	}

	return 0;
}

void EagleApplication::handleAppCommand(android_app* app, int32_t cmd)
{

	assert(app->userData != nullptr);
	EagleApplication* application = reinterpret_cast<EagleApplication*>(app->userData);
	auto handler = application->getHandler();

	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		LOGD("APP_CMD_SAVE_STATE");
		/*
		eagleApp->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)eagleApp->app->savedState) = eagleApp->state;
		eagleApp->app->savedStateSize = sizeof(struct saved_state);
		*/
		break;
	case APP_CMD_INIT_WINDOW:
		LOGD("APP_CMD_INIT_WINDOW");

		if (handler.androidApp->window != nullptr)
		{
			application->initialize();
			if (!application->_prepared) {
				LOGE("Could not initialize Vulkan, exiting!");
				handler.androidApp->destroyRequested = 1;
			}
		}
		else
		{
			LOGE("No window assigned!");
		}
		break;
	case APP_CMD_LOST_FOCUS:
		LOGD("APP_CMD_LOST_FOCUS");
		application->appPause();
		break;
	case APP_CMD_GAINED_FOCUS:
		LOGD("APP_CMD_GAINED_FOCUS");
		application->appFocus();
		break;
	case APP_CMD_DESTROY:
		application->appExit();
		break;
	case APP_CMD_TERM_WINDOW:
		// Window is hidden or closed, clean up resources
		LOGD("APP_CMD_TERM_WINDOW");
		/*if (application->_prepared) {
            application->appExit();
		}*/
		break;
	}
}
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
#if defined(EXAMPLE_XCODE_GENERATED)
@interface AppDelegate : NSObject<NSApplicationDelegate>
{
}

@end

@implementation AppDelegate
{
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return YES;
}

@end

static CVReturn displayLinkOutputCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* inNow,
	const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut,
	void* displayLinkContext)
{
	@autoreleasepool
	{
		auto eagleApp = static_cast<EagleApplication*>(displayLinkContext);
			eagleApp->displayLinkOutputCb();
	}
	return kCVReturnSuccess;
}

@interface View : NSView<NSWindowDelegate>
{
	@public
		EagleApplication* eagleApp;
}

@end

@implementation View
{
	CVDisplayLinkRef displayLink;
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame : (frameRect)];
	if (self)
	{
		self.wantsLayer = YES;
		self.layer = [CAMetalLayer layer];
	}
	return self;
}

- (void)viewDidMoveToWindow
{
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	CVDisplayLinkSetOutputCallback(displayLink, &displayLinkOutputCallback, eagleApp);
	CVDisplayLinkStart(displayLink);
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)keyDown:(NSEvent*)event
{
	switch (event.keyCode)
	{
	case kVK_ANSI_P:
		// eagleApp->paused = !eagleApp->paused;
		break;
	case kVK_Escape:
		[NSApp terminate : nil] ;
		break;
	case kVK_ANSI_W:
		// eagleApp->camera.keys.up = true;
		break;
	case kVK_ANSI_S:
		// eagleApp->camera.keys.down = true;
		break;
	case kVK_ANSI_A:
		// eagleApp->camera.keys.left = true;
		break;
	case kVK_ANSI_D:
		// eagleApp->camera.keys.right = true;
		break;
	default:
		break;
	}
}

- (void)keyUp:(NSEvent*)event
{
	switch (event.keyCode)
	{
	case kVK_ANSI_W:
		// eagleApp->camera.keys.up = false;
		break;
	case kVK_ANSI_S:
		// eagleApp->camera.keys.down = false;
		break;
	case kVK_ANSI_A:
		// eagleApp->camera.keys.left = false;
		break;
	case kVK_ANSI_D:
		// eagleApp->camera.keys.right = false;
		break;
	default:
		break;
	}
}

- (NSPoint)getMouseLocalPoint:(NSEvent*)event
{
	NSPoint location = [event locationInWindow];
	NSPoint point = [self convertPoint : location fromView : nil];
	point.y = self.frame.size.height - point.y;
	return point;
}

- (void)mouseDown:(NSEvent*)event
{
	auto point = [self getMouseLocalPoint : event];
	// eagleApp->mousePos = glm::vec2(point.x, point.y);
	// eagleApp->mouseButtons.left = true;
}

- (void)mouseUp:(NSEvent*)event
{
	auto point = [self getMouseLocalPoint : event];
	// eagleApp->mousePos = glm::vec2(point.x, point.y);
	// eagleApp->mouseButtons.left = false;
}

- (void)otherMouseDown:(NSEvent*)event
{
	// eagleApp->mouseButtons.right = true;
}

- (void)otherMouseUp : (NSEvent*)event
{
	// eagleApp->mouseButtons.right = false;
}

- (void)mouseDragged : (NSEvent*)event
{
	auto point = [self getMouseLocalPoint : event];
	// eagleApp->mouseDragged(point.x, point.y);
}

- (void)mouseMoved:(NSEvent*)event
{
	auto point = [self getMouseLocalPoint : event];
	// eagleApp->mouseDragged(point.x, point.y);
}

- (void)scrollWheel:(NSEvent*)event
{
	short wheelDelta = [event deltaY];
	// eagleApp->camera.translate(glm::vec3(0.0f, 0.0f,
	//	-(float)wheelDelta * 0.05f * eagleApp->camera.movementSpeed));
}

- (NSSize)windowWillResize:(NSWindow*)sender toSize : (NSSize)frameSize
{
	CVDisplayLinkStop(displayLink);
	eagleApp->windowWillResize(frameSize.width, frameSize.height);
	return frameSize;
}

- (void)windowDidResize:(NSNotification*)notification
{
	eagleApp->windowDidResize();
	CVDisplayLinkStart(displayLink);
}

- (BOOL)windowShouldClose:(NSWindow*)sender
{
	return TRUE;
}

- (void)windowWillClose : (NSNotification*)notification
{
	CVDisplayLinkStop(displayLink);
}

@end
#endif

void* EagleApplication::setupWindow(void* view, uint32_t sw, uint32_t sh)
{
#if defined(EXAMPLE_XCODE_GENERATED)
	NSApp = [NSApplication sharedApplication];
	[NSApp setActivationPolicy : NSApplicationActivationPolicyRegular] ;
	[NSApp setDelegate : [AppDelegate new] ] ;
	const ContextInfo& info = _renderer->getContext()->getContextInfo();
	const auto kContentRect = NSMakeRect(0.0f, 0.0f, info.width, info.height);
	const auto kWindowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

	auto window = [[NSWindow alloc]initWithContentRect:kContentRect
		styleMask : kWindowStyle
		backing : NSBackingStoreBuffered
		defer : NO];
	[window setTitle : @(info.applicationName.c_str())] ;
	[window setAcceptsMouseMovedEvents : YES] ;
	[window center] ;
	[window makeKeyAndOrderFront : nil] ;

	auto nsView = [[View alloc]initWithFrame:kContentRect];
	nsView->eagleApp = this;
	[window setDelegate : nsView] ;
	[window setContentView : nsView] ;
	_handler.view = (__bridge void*)nsView;
#else
	_handler.view = view;
#if defined(USE_PLATFORM_IOS_MVK)
    _handler.screenWidth = sw;
    _handler.screenHeight = sh;
#endif
    
#endif
	return view;
}

void EagleApplication::displayLinkOutputCb()
{
	if (_prepared)
		nextFrame();
}

void EagleApplication::mouseDragged(float x, float y)
{
	handleMouseMove(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
}

void EagleApplication::windowWillResize(float x, float y)
{
	_resizing = true;
	if (_prepared)
	{
		_destWidth = x;
		_destHeight = y;
		windowResize();
	}
}

void EagleApplication::windowDidResize()
{
	_resizing = false;
}
#else
void EagleApplication::setupWindow()
{
}
#endif
