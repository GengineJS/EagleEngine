#pragma once
#include <EagleRenderer.h>
#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingAPI.h>
#elif defined(USE_PLATFORM_ANDROID_KHR)
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <sys/system_properties.h>
#endif
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/Context.h>
#include <unordered_map>
#include <assert.h>
#include <utils/Keycodes.hpp>
#include <imgui.h>
#include <chrono>

namespace eg {
struct MouseButton {
	bool left{ false };
	bool right{ false };
	bool middle{ false };
};
class EagleApplication : public std::enable_shared_from_this<EagleApplication> {
public:
	EagleApplication() = default;
	EagleApplication(std::shared_ptr<EagleRenderer> renderer);
	virtual ~EagleApplication() = default;
	inline std::string getWindowTitle() const { return _renderer->getContext()->getContextInfo().applicationName; };
	glm::vec2 mousePos;
	MouseButton mouseButton;
#if defined(_WIN32)
	void setupConsole(std::string title);
	void setupDPIAwareness();
	HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#elif defined(USE_PLATFORM_ANDROID_KHR)
	static int32_t handleAppInput(struct android_app* app, AInputEvent* event);
	static void handleAppCommand(android_app* app, int32_t cmd);
	inline void setAndroidApp(android_app* app) { _handler.androidApp = app; };
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
	void* setupWindow(void* view, uint32_t sw = 0, uint32_t sh = 0);
	void displayLinkOutputCb();
	void mouseDragged(float x, float y);
	void windowWillResize(float x, float y);
	void windowDidResize();
#else
	void setupWindow();
#endif
	virtual void keyPressed(uint32_t);
	virtual void mouseMoved(double x, double y, bool& handled);
	inline const AppHandler& getHandler() const { return _handler; }
	inline bool getPrepared() const { return _prepared; }
	void handleMouseMove(int32_t x, int32_t y);
	void windowResize();
	void initialize();
	void appExit();
	void appPause();
	void appFocus();
	void renderLoop();
	void nextFrame();
private:
	bool _prepared{ false };
	bool _resizing{ false };
	bool _paused{ false };
	bool _isInit{false};
	// Frame counter to display fps
	uint32_t _frameCounter = 0;
	uint32_t _lastFPS = 0;
	/** @brief Last frame time measured using a high performance timer (if available) */
	float _frameTimer = 1.0f;
	float _timer = 0.0f;
	// Multiplier for speeding up (or slowing down) the global timer
	float _timerSpeed = 0.25f;
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimestamp;
	uint32_t _destWidth{ 0 };
	uint32_t _destHeight{ 0 };
	AppHandler _handler{};
	std::shared_ptr<EagleRenderer> _renderer{nullptr};
};
}
using namespace eg;
#if defined(_WIN32)
// Windows entry point
#define EAGLE_MAIN()												\
extern std::shared_ptr<EagleRenderer> EagleSetup();					\
std::shared_ptr<EagleApplication> application{ nullptr };			\
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) \
{																	\
	if (application != NULL)										\
	{																\
		application->handleMessages(hWnd, uMsg, wParam, lParam);	\
	}																\
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));			    \
}																    \
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)	\
{																	\
	std::shared_ptr<EagleRenderer> renderer = EagleSetup();			\
	application = std::make_shared<EagleApplication>(renderer);		\
	application->setupWindow(hInstance, WndProc);					\
	application->initialize();										\
	application->renderLoop();										\
	return 0;														\
}																									
#elif defined(__ANDROID__)
// Android entry point
#define EAGLE_MAIN()												\
extern std::shared_ptr<EagleRenderer> EagleSetup();					\
std::shared_ptr<EagleApplication> application{ nullptr };			\
void android_main(android_app* state)								\
{																	\
                                                                    \
	std::shared_ptr<EagleRenderer> renderer = EagleSetup();			\
	application = std::make_shared<EagleApplication>(renderer);		\
	state->userData = application.get();							\
	state->onAppCmd = EagleApplication::handleAppCommand;			\
	state->onInputEvent = EagleApplication::handleAppInput;			\
	application->setAndroidApp(state);                              \
	application->renderLoop();										\
}
#elif (defined(USE_PLATFORM_MACOS_MVK) || defined(USE_PLATFORM_IOS_MVK))
#if defined(EXAMPLE_XCODE_GENERATED)
// IOS/Mac entry point
#define EAGLE_MAIN()												\
extern std::shared_ptr<EagleRenderer> EagleSetup();	    			\
std::shared_ptr<EagleApplication> application{ nullptr };			\
int main(const int argc, const char* argv[])						\
{                                                                   \
	std::shared_ptr<EagleRenderer> renderer = EagleSetup();			\
    application = std::make_shared<EagleApplication>(renderer);     \
	application->setupWindow(nullptr);								\
	application->initialize();										\
	application->renderLoop();								    	\
	return 0;                                                       \
}
#else
#define EAGLE_MAIN()
#endif
#endif
