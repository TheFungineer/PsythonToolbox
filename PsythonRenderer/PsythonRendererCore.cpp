/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#include "PsythonRendererCore.hpp"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cassert>

namespace psy {
	
PsythonRendererCore::PsythonRendererCore()
	: mIsRunning(false)
{
	
}

PsythonRendererCore::~PsythonRendererCore()
{
	dispose();
}

bool PsythonRendererCore::initialize()
{

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		return false;
	}
	return true;
}

void PsythonRendererCore::dispose()
{
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		mClientWindows[i].reset();
	}
	SDL_Quit();
}

uint32_t PsythonRendererCore::createWindow()
{
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (!mClientWindows[i]) {
			mClientWindows[i].reset(new (std::nothrow) PsythonWindow);
			if (!mClientWindows[i]) {
				return INVALID_PSYTHONWINDOW_UID;
			}
			return mClientWindows[i]->getUniqueId();
		}
	}
	return INVALID_PSYTHONWINDOW_UID;
}

psy::PsythonWindow* PsythonRendererCore::getWindowByUId(uint32_t inUId) const
{
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (mClientWindows[i] && mClientWindows[i]->getUniqueId() == inUId) {
			return mClientWindows[i].get();
		}
	}
	return nullptr;
}

void PsythonRendererCore::closeWindow(uint32_t inUId)
{
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (mClientWindows[i] && mClientWindows[i]->getUniqueId() == inUId) {
			mClientWindows[i].reset();
			break;
		}
	}
}

void PsythonRendererCore::pumpNativeEvents()
{
	SDL_PumpEvents();
	SDL_Event lSdlEvent;
	while (SDL_PollEvent(&lSdlEvent) != 0) {
		PsythonNativeEvent lPsythonEvent;
		switch (lSdlEvent.type) {
			case SDL_QUIT: {
				lPsythonEvent.mType = PsythonNativeEvent::QUIT_REQUESTED;
				notify(lPsythonEvent);
				break;
			}
			case SDL_WINDOWEVENT: {
				switch (lSdlEvent.window.event) {
					case SDL_WINDOWEVENT_SHOWN: {
						lPsythonEvent.mType = PsythonNativeEvent::WINDOW_VISIBILITY_CHANGED;
						lPsythonEvent.mWindowUId = lSdlEvent.window.windowID;
						lPsythonEvent.mHasWindowFlag = true;
						notify(lPsythonEvent);
						break;
					}
					case SDL_WINDOWEVENT_HIDDEN: {
						lPsythonEvent.mType = PsythonNativeEvent::WINDOW_VISIBILITY_CHANGED;
						lPsythonEvent.mWindowUId = lSdlEvent.window.windowID;
						lPsythonEvent.mHasWindowFlag = false;
						notify(lPsythonEvent);
						break;
					}
					case SDL_WINDOWEVENT_SIZE_CHANGED: {
						lPsythonEvent.mType = PsythonNativeEvent::WINDOW_SIZE_CHANGED;
						lPsythonEvent.mWindowUId = lSdlEvent.window.windowID;
						lPsythonEvent.mWindowWidth = lSdlEvent.window.data1;
						lPsythonEvent.mWindowHeight = lSdlEvent.window.data2;
						notify(lPsythonEvent);
						break;
					}
					case SDL_WINDOWEVENT_FOCUS_GAINED: {
						lPsythonEvent.mType = PsythonNativeEvent::WINDOW_FOCUS_CHANGED;
						lPsythonEvent.mWindowUId = lSdlEvent.window.windowID;
						lPsythonEvent.mHasWindowFlag = true;
						notify(lPsythonEvent);
						break;
					}
					case SDL_WINDOWEVENT_FOCUS_LOST: {
						lPsythonEvent.mType = PsythonNativeEvent::WINDOW_FOCUS_CHANGED;
						lPsythonEvent.mWindowUId = lSdlEvent.window.windowID;
						lPsythonEvent.mHasWindowFlag = false;
						notify(lPsythonEvent);
						break;
					}
					case SDL_WINDOWEVENT_CLOSE: {
						lPsythonEvent.mType = PsythonNativeEvent::WINDOW_CLOSE_REQUESTED;
						lPsythonEvent.mWindowUId = lSdlEvent.window.windowID;
						notify(lPsythonEvent);
						break;
					}
					default: break;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				lPsythonEvent.mType = PsythonNativeEvent::MOUSE_MOVED;
				lPsythonEvent.mMouseWindowUId = lSdlEvent.motion.windowID;
				lPsythonEvent.mDownMouseButtons = lSdlEvent.motion.state;
				lPsythonEvent.mIsRelative = SDL_GetRelativeMouseMode() == SDL_TRUE;
				if (lPsythonEvent.mIsRelative) {
					lPsythonEvent.mCursorX = lSdlEvent.motion.xrel;
					lPsythonEvent.mCursorY = lSdlEvent.motion.yrel;
				}
				else {
					lPsythonEvent.mCursorX = lSdlEvent.motion.x;
					lPsythonEvent.mCursorY = lSdlEvent.motion.y;
				}
				notify(lPsythonEvent);
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				lPsythonEvent.mType = PsythonNativeEvent::MOUSE_DOWN;
				lPsythonEvent.mMouseWindowUId = lSdlEvent.button.windowID;
				switch (lSdlEvent.button.button) {
					case SDL_BUTTON_LEFT:	lPsythonEvent.mDownMouseButtons = SDL_BUTTON_LMASK; break;
					case SDL_BUTTON_MIDDLE:	lPsythonEvent.mDownMouseButtons = SDL_BUTTON_MMASK; break;
					case SDL_BUTTON_RIGHT:	lPsythonEvent.mDownMouseButtons = SDL_BUTTON_RMASK; break;
					case SDL_BUTTON_X1:		lPsythonEvent.mDownMouseButtons = SDL_BUTTON_X1MASK; break;
					case SDL_BUTTON_X2:		lPsythonEvent.mDownMouseButtons = SDL_BUTTON_X2MASK; break;
					default: assert(false); break;
				}
				lPsythonEvent.mIsRelative = SDL_GetRelativeMouseMode() == SDL_TRUE;
				if (!lPsythonEvent.mIsRelative) {
					lPsythonEvent.mCursorX = lSdlEvent.button.x;
					lPsythonEvent.mCursorY = lSdlEvent.button.y;
				}
				else {
					lPsythonEvent.mCursorX = -1;
					lPsythonEvent.mCursorY = -1;
				}
				notify(lPsythonEvent);
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				lPsythonEvent.mType = PsythonNativeEvent::MOUSE_UP;
				lPsythonEvent.mMouseWindowUId = lSdlEvent.button.windowID;
				switch (lSdlEvent.button.button) {
					case SDL_BUTTON_LEFT:	lPsythonEvent.mDownMouseButtons = SDL_BUTTON_LMASK; break;
					case SDL_BUTTON_MIDDLE:	lPsythonEvent.mDownMouseButtons = SDL_BUTTON_MMASK; break;
					case SDL_BUTTON_RIGHT:	lPsythonEvent.mDownMouseButtons = SDL_BUTTON_RMASK; break;
					case SDL_BUTTON_X1:		lPsythonEvent.mDownMouseButtons = SDL_BUTTON_X1MASK; break;
					case SDL_BUTTON_X2:		lPsythonEvent.mDownMouseButtons = SDL_BUTTON_X2MASK; break;
					default: assert(false); break;
				}
				lPsythonEvent.mIsRelative = SDL_GetRelativeMouseMode() == SDL_TRUE;
				if (!lPsythonEvent.mIsRelative) {
					lPsythonEvent.mCursorX = lSdlEvent.button.x;
					lPsythonEvent.mCursorY = lSdlEvent.button.y;
				}
				else {
					lPsythonEvent.mCursorX = -1;
					lPsythonEvent.mCursorY = -1;
				}
				notify(lPsythonEvent);
				break;
			}
			case SDL_KEYDOWN: {
				if (lSdlEvent.key.repeat == 0) {
					lPsythonEvent.mType = PsythonNativeEvent::KEY_DOWN;
					lPsythonEvent.mKeyWindowUId = lSdlEvent.key.windowID;
					lPsythonEvent.mKeyName = SDL_GetKeyName(lSdlEvent.key.keysym.sym);
					lPsythonEvent.mKeyScancode = lSdlEvent.key.keysym.scancode;
					lPsythonEvent.mWithShift = (lSdlEvent.key.keysym.mod & KMOD_SHIFT);
					lPsythonEvent.mWithAlt = (lSdlEvent.key.keysym.mod & KMOD_ALT);
					lPsythonEvent.mWithCtrl = (lSdlEvent.key.keysym.mod & KMOD_CTRL);
					notify(lPsythonEvent);
				}
				break;
			}
			case SDL_KEYUP: {
				if (lSdlEvent.key.repeat == 0) {
					lPsythonEvent.mType = PsythonNativeEvent::KEY_UP;
					lPsythonEvent.mKeyWindowUId = lSdlEvent.key.windowID;
					lPsythonEvent.mKeyName = SDL_GetKeyName(lSdlEvent.key.keysym.sym);
					lPsythonEvent.mKeyScancode = lSdlEvent.key.keysym.scancode;
					lPsythonEvent.mWithShift = (lSdlEvent.key.keysym.mod & KMOD_SHIFT);
					lPsythonEvent.mWithAlt = (lSdlEvent.key.keysym.mod & KMOD_ALT);
					lPsythonEvent.mWithCtrl = (lSdlEvent.key.keysym.mod & KMOD_CTRL);
					notify(lPsythonEvent);
				}
				break;
			}
			default: break;
		}
	}
}

} // namespace psy