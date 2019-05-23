/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#pragma once
#include <Python.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdint>

namespace psy {

class PsythonWindow
{
public:
	PsythonWindow();
	~PsythonWindow();
	void show();
	void hide();
	void bringToFront();
	void setTitle(const char* inTitle);
	const char* getTitle() const;
	void setX(uint32_t inX);
	void setY(uint32_t inY);
	uint32_t getX() const;
	uint32_t getY() const;
	void setFullScreen(bool inFullscreenMode);
	void minimize();
	void restore();
	void maximize();
	void setWidth(uint32_t inWidth);
	void setHeight(uint32_t inHeight);
	uint32_t getHeight() const;
	uint32_t getWidth() const;
	void setMouseClip(bool inIsClipping);
	uint32_t getUniqueId() const;
		
	SDL_Window* getSdlWindow() const;
	
private:
	SDL_Window* mSDLWindow;
};

/*////////////////////////////////////*/

struct PsythonWindowPyHandle
{
    PyObject_HEAD
	class PsythonRendererCore* mCore;
    uint32_t mWindowUID;
};

extern PyTypeObject* getPsythonWindowPyTypeObject();

} // namespace psy
