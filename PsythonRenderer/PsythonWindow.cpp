#include "PsythonWindow.hpp"
#include "PsythonRendererCore.hpp"

namespace psy {

PsythonWindow::PsythonWindow() 
	: mSDLWindow(nullptr)
{
	mSDLWindow = SDL_CreateWindow("", 
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								1920, 
								1080, 
								SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	SDL_SetWindowMinimumSize(mSDLWindow, 640, 480);
}

PsythonWindow::~PsythonWindow() 
{
	if (mSDLWindow) {
		SDL_DestroyWindow(mSDLWindow);
		mSDLWindow = nullptr;
	}
}

void PsythonWindow::show() 
{	
	SDL_ShowWindow(mSDLWindow);	
}


void PsythonWindow::hide() 
{
	SDL_HideWindow(mSDLWindow);
}

void PsythonWindow::bringToFront()
{
	SDL_RaiseWindow(mSDLWindow);
}

void PsythonWindow::setTitle(const char* inTitle) 
{
	SDL_SetWindowTitle(mSDLWindow, inTitle);
}

const char* PsythonWindow::getTitle() const
{
	return SDL_GetWindowTitle(mSDLWindow);
}

void PsythonWindow::setX(uint32_t inX)
{
	SDL_SetWindowPosition(mSDLWindow, inX, getY());
}

void PsythonWindow::setY(uint32_t inY)
{
	SDL_SetWindowPosition(mSDLWindow, getX(), inY);
}

uint32_t PsythonWindow::getX() const
{
	int lX, lY;
	SDL_GetWindowPosition(mSDLWindow, &lX, &lY);
	return lX;
}

uint32_t PsythonWindow::getY() const
{
	int lX, lY;
	SDL_GetWindowPosition(mSDLWindow, &lX, &lY);
	return lY;
}

void PsythonWindow::setFullScreen(bool inFullscreenMode) 
{
	if (!inFullscreenMode) {
		SDL_SetWindowFullscreen(mSDLWindow, 0);
	}
	else {
		SDL_SetWindowFullscreen(mSDLWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
}

void PsythonWindow::minimize()
{
	SDL_MinimizeWindow(mSDLWindow);
}

void PsythonWindow::restore()
{
	SDL_RestoreWindow(mSDLWindow);
}

void PsythonWindow::maximize()
{
	SDL_MaximizeWindow(mSDLWindow);
}

void PsythonWindow::setWidth(uint32_t inWidth)
{
	SDL_SetWindowSize(mSDLWindow, static_cast<int>(inWidth), static_cast<int>(getHeight()));
}

void PsythonWindow::setHeight(uint32_t inHeight)
{
	SDL_SetWindowSize(mSDLWindow, static_cast<int>(getWidth()), static_cast<int>(inHeight));
}

uint32_t PsythonWindow::getHeight() const
{
	int lWidth, lHeight;
	SDL_GetWindowSize(mSDLWindow, &lWidth, &lHeight);
	return lHeight;
}

uint32_t PsythonWindow::getWidth() const
{
	int lWidth, lHeight;
	SDL_GetWindowSize(mSDLWindow, &lWidth, &lHeight);
	return lWidth;
}

void PsythonWindow::setMouseClip(bool inIsClipping)
{
	SDL_SetWindowGrab(mSDLWindow, inIsClipping ? SDL_TRUE : SDL_FALSE);
}

uint32_t PsythonWindow::getUniqueId() const
{
	return SDL_GetWindowID(mSDLWindow);
}

SDL_Window* PsythonWindow::getSdlWindow() const
{
	return mSDLWindow;
}

/*//////////////////////////////*/

namespace {
	
extern "C" {

PyObject* winShow(PyObject* inSelf, PyObject* inArgs)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->show();
    Py_RETURN_NONE;
}

PyObject* winHide(PyObject* inSelf, PyObject* inArgs)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->hide();
    Py_RETURN_NONE;
}

PyObject* winBringToFront(PyObject* inSelf, PyObject* inArgs)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->bringToFront();
    Py_RETURN_NONE;
}

PyObject* winMaximize(PyObject* inSelf, PyObject* inArgs)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->maximize();
    Py_RETURN_NONE;
}

PyObject* winMinimize(PyObject* inSelf, PyObject* inArgs)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->minimize();
    Py_RETURN_NONE;
}

PyObject* winRestore(PyObject* inSelf, PyObject* inArgs)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->restore();
    Py_RETURN_NONE;
}

PyObject* winSetFullscreen(PyObject* inSelf, PyObject* inArgs)
{
	int lIsFullscreen = 0;
	if (!PyArg_ParseTuple(inArgs, "i", &lIsFullscreen)) {
		PyErr_SetString(PyExc_TypeError, "setFullscreen expects a boolean argument: True or False");
		return NULL;
	}
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->setFullScreen(lIsFullscreen != 0);
    Py_RETURN_NONE;
}

PyObject* winSetMouseClip(PyObject* inSelf, PyObject* inArgs)
{
	int lHasMouseClip = 0;
	if (!PyArg_ParseTuple(inArgs, "i", &lHasMouseClip)) {
		PyErr_SetString(PyExc_TypeError, "setMouseClip expects a boolean argument: True or False");
		return NULL;
	}
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
	lWindow->setMouseClip(lHasMouseClip != 0);
    Py_RETURN_NONE;
}

PyMethodDef PsythonWindowPy_methods[] = {
	{"show", winShow, METH_NOARGS,
     "Make the window visible on screen"
    },
	{"hide", winHide, METH_NOARGS,
     "Make the window not visible on screen"
    },
	{"bringToFront", winBringToFront, METH_NOARGS,
     "Bring the window in front of any other windows"
    },
	{"maximize", winMaximize, METH_NOARGS,
     "Set the window in its maximized state and take the size of the desktop"
    },
	{"minimize", winMinimize, METH_NOARGS,
     "Set the window in its minimized state with os specific behaviours"
    },
	{"restore", winRestore, METH_NOARGS,
     "Restore the window from the maximized or minimized state."
    },
    {"setFullscreen", winSetFullscreen, METH_VARARGS,
     "Make the window borderless and take the size of the whole screen"
    },
	{"setMouseClip", winSetMouseClip, METH_VARARGS,
     "Mouse clip will restrain the mouse cursor inside the window."
    },
    {NULL}  /* Sentinel */
};

int winSetTitle(PyObject* inSelf, PyObject* inValue, void*)
{
    if (inValue == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the title attribute");
        return -1;
    }
    if (!PyUnicode_Check(inValue)) {
        PyErr_SetString(PyExc_TypeError, "The window title attribute value must be a string");
        return -1;
    }
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return -1;
	}
	lWindow->setTitle(PyUnicode_AsUTF8(inValue));
    return 0;
}

PyObject* winGetTitle(PyObject* inSelf, void*)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
    return PyUnicode_FromString(lWindow->getTitle());
}

int winSetX(PyObject* inSelf, PyObject* inValue, void*)
{
    if (inValue == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the x coordinate attribute");
        return -1;
    }
    if (!PyLong_Check(inValue)) {
        PyErr_SetString(PyExc_TypeError, "The window x coordinate attribute value must be an integer");
        return -1;
    }
 	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return -1;
	}
	lWindow->setX(PyLong_AsUnsignedLong(inValue));
    return 0;
}

PyObject* winGetX(PyObject* inSelf, void*)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
    return PyLong_FromUnsignedLong(lWindow->getX());
}

int winSetY(PyObject* inSelf, PyObject* inValue, void*)
{
    if (inValue == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the y coordinate attribute");
        return -1;
    }
    if (!PyLong_Check(inValue)) {
        PyErr_SetString(PyExc_TypeError, "The window y coordinate attribute value must be an integer");
        return -1;
    }
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return -1;
	}
    lWindow->setY(PyLong_AsUnsignedLong(inValue));
    return 0;
}

PyObject* winGetY(PyObject* inSelf, void*)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
    return PyLong_FromUnsignedLong(lWindow->getY());
}

int winSetWidth(PyObject* inSelf, PyObject* inValue, void*)
{
    if (inValue == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the width coordinate attribute");
        return -1;
    }
    if (!PyLong_Check(inValue)) {
        PyErr_SetString(PyExc_TypeError, "The window width attribute value must be an integer");
        return -1;
    }
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return -1;
	}
    lWindow->setWidth(PyLong_AsUnsignedLong(inValue));
    return 0;
}

PyObject* winGetWidth(PyObject* inSelf, void*)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
    return PyLong_FromUnsignedLong(lWindow->getWidth());
}

int winSetHeight(PyObject* inSelf, PyObject* inValue, void*)
{
    if (inValue == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the height coordinate attribute");
        return -1;
    }
    if (!PyLong_Check(inValue)) {
        PyErr_SetString(PyExc_TypeError, "The window height attribute value must be an integer");
        return -1;
    }
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return -1;
	}
    lWindow->setHeight(PyLong_AsUnsignedLong(inValue));
    return 0;
}

PyObject* winGetHeight(PyObject* inSelf, void*)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
    return PyLong_FromUnsignedLong(lWindow->getHeight());
}

PyObject* winGetUId(PyObject* inSelf, void*)
{
	psy::PsythonWindow* lWindow = ((psy::PsythonWindowPyHandle*)inSelf)->mCore->getWindowByUId(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
	if (!lWindow) {
		PyErr_SetString(PyExc_TypeError, "The referenced window no longer exists");
		return NULL;
	}
    return PyLong_FromUnsignedLong(lWindow->getUniqueId());
}

PyGetSetDef PsythonWindowPy_getsetters[] = {
    {"title", winGetTitle, winSetTitle, "The window title, appearing in the bar at its top", NULL},
	{"x", winGetX, winSetX, "The window horizontal position on the screen", NULL},
	{"y", winGetY, winSetY, "The window vertical position on the screen", NULL},
	{"width", winGetWidth, winSetWidth, "The window width on the screen", NULL},
	{"height", winGetHeight, winSetHeight, "The window height on the screen", NULL},
	{"UId", winGetUId, NULL, "The window unique ID", NULL},
    {NULL}  /* Sentinel */
};

void PsythonWindowPy_dealloc(PyObject* inSelf)
{
	((psy::PsythonWindowPyHandle*)inSelf)->mCore->closeWindow(((psy::PsythonWindowPyHandle*)inSelf)->mWindowUID);
    Py_TYPE(inSelf)->tp_free(inSelf);
}

PyTypeObject PsythonWindowPy_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PsythonRenderer.PsythonWindow", /* tp_name */
    sizeof(psy::PsythonWindowPyHandle), /* tp_basicsize */
    0,                         /* tp_itemsize */
    PsythonWindowPy_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,    		/* tp_flags */
    "A renderable window",       /* tp_doc */
    0,  						/* tp_traverse */
    0,           				/* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PsythonWindowPy_methods,    /* tp_methods */
    0,             				/* tp_members */
    PsythonWindowPy_getsetters, /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,      					/* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
};

} // extern "C"

} // anonymous namespace

/*////////////////////////////////////*/

PyTypeObject* getPsythonWindowPyTypeObject()
{
	return &PsythonWindowPy_type;
}

} // namespace psy