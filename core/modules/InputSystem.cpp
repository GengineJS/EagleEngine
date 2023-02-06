#include <modules/InputSystem.hpp>

namespace eg {
	void InputSystem::execute(ModuleStage curr)
	{
		EagleWindow::Get()->onKey().Add([&](uint32_t code, bool isDown) {
			_onKey(static_cast<KeyCode>(code), isDown);
		});
		EagleWindow::Get()->onMouse().Add([&](uint32_t code, bool isDown, int32_t x, int32_t y) {
			_onMouse(static_cast<MouseButton>(code), isDown, x, y);
		});
		EagleWindow::Get()->onMouseWheel().Add([&](short delta) {
			_onMouseWheel(delta);
		});
		EagleWindow::Get()->onMouseMove().Add([&](int32_t x, int32_t y) {
			_onMouseMove(x, y);
		});
	}
}

