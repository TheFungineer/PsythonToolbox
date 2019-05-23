/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#pragma once
#include "PsythonWindow.hpp"
#include "PsythonEvent.hpp"
#include <cstdint>
#include <memory>

namespace psy {
			
struct PsythonNativeEvent : public PsythonEvent
{
	enum EventType
	{
		QUIT_REQUESTED,
		KEY_DOWN,
		KEY_UP,
		MOUSE_MOVED,
		MOUSE_DOWN,
		MOUSE_UP,
		WINDOW_VISIBILITY_CHANGED,
		WINDOW_FOCUS_CHANGED,
		WINDOW_SIZE_CHANGED,
		WINDOW_CLOSE_REQUESTED,
	};
	
	union {
		struct { // keyboard events
			uint32_t mKeyWindowUId;
			const char* mKeyName;
			int32_t mKeyScancode;
			bool mWithShift;
			bool mWithAlt;
			bool mWithCtrl;
		};
		struct { // mouse events
			uint32_t mMouseWindowUId;
			uint32_t mDownMouseButtons;
			int32_t mCursorX;
			int32_t mCursorY;
			bool mIsRelative;
		};
		struct { // window events
			uint32_t mWindowUId;
			bool mHasWindowFlag;
			uint32_t mWindowWidth;
			uint32_t mWindowHeight;
		};
	};
};

class PsythonRendererCore : public PsythonEventNotifier
{
public:
	enum : uint32_t { 
		MAX_WINDOW_COUNT = 4u,
		INVALID_PSYTHONWINDOW_UID = UINT32_MAX,
	};
public:
	PsythonRendererCore();
	~PsythonRendererCore();
	bool initialize();
	void dispose();
	int run(); 
	
	uint32_t createWindow();
	psy::PsythonWindow* getWindowByUId(uint32_t inUId) const;
	void closeWindow(uint32_t inWindowUniqueId);
	
	void pumpNativeEvents();
private:
	
private:
	std::unique_ptr<psy::PsythonWindow> mClientWindows[MAX_WINDOW_COUNT];
	bool mIsRunning;
};
	
} // namespace psy
