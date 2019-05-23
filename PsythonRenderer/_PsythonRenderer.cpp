#include <Python.h>
#include <cstdio>
#include <memory>
#include "PsythonRendererCore.hpp"

namespace {
	
// Internal Variables
	
std::unique_ptr<psy::PsythonRendererCore> gPsythonRendererCore;
uint32_t gModuleInitializationRefCount = 0;

/*//////////////////////////////*/

// Internal functions

void sOnNativeEvent(void* inContext, const psy::PsythonEvent& inEvent)
{
	const auto& lEvent = static_cast<const psy::PsythonNativeEvent&>(inEvent);
	PyObject* lCallback;
	PyObject* lCallbackContext;
    if (!PyArg_ParseTuple(static_cast<PyObject*>(inContext), "OO", &lCallback, &lCallbackContext)) {
		PyErr_SetString(PyExc_TypeError, "Internal error. Could not register callback. ");
        return;
	}
	
	PyObject* lArglist = nullptr;
	PyObject* lResult = nullptr;
	switch (lEvent.mType) {
		case psy::PsythonNativeEvent::QUIT_REQUESTED: {
			lArglist = Py_BuildValue("(O)", lCallbackContext);
			break;
		}
		case psy::PsythonNativeEvent::WINDOW_VISIBILITY_CHANGED: [[fallthrough]];
		case psy::PsythonNativeEvent::WINDOW_FOCUS_CHANGED: {
			lArglist = Py_BuildValue("(OIN)", lCallbackContext, lEvent.mWindowUId, PyBool_FromLong(lEvent.mHasWindowFlag));
			break;
		}
		case psy::PsythonNativeEvent::WINDOW_SIZE_CHANGED: {
			lArglist = Py_BuildValue("(OIII)", lCallbackContext, lEvent.mWindowUId, lEvent.mWindowWidth, lEvent.mWindowHeight);
			break;
		}
		case psy::PsythonNativeEvent::WINDOW_CLOSE_REQUESTED: {
			lArglist = Py_BuildValue("(OI)", lCallbackContext, lEvent.mWindowUId);
			break;
		}
		case psy::PsythonNativeEvent::MOUSE_MOVED: {
			lArglist = Py_BuildValue("(OIiiNI)", 
									lCallbackContext, 
									lEvent.mMouseWindowUId,
									lEvent.mCursorX,
									lEvent.mCursorY,
									PyBool_FromLong(lEvent.mIsRelative),
									lEvent.mDownMouseButtons);
			break;
		}
		case psy::PsythonNativeEvent::MOUSE_DOWN: {
			lArglist = Py_BuildValue("(OIiiI)", 
									lCallbackContext, 
									lEvent.mMouseWindowUId,
									lEvent.mCursorX,
									lEvent.mCursorY,
									lEvent.mDownMouseButtons);
			break;
		}
		case psy::PsythonNativeEvent::MOUSE_UP: {
			lArglist = Py_BuildValue("(OIiiI)", 
									lCallbackContext, 
									lEvent.mMouseWindowUId,
									lEvent.mCursorX,
									lEvent.mCursorY,
									lEvent.mDownMouseButtons);
			break;
		}
		case psy::PsythonNativeEvent::KEY_DOWN: {
			lArglist = Py_BuildValue("(OIsiNNN)", 
									lCallbackContext, 
									lEvent.mKeyWindowUId,
									lEvent.mKeyName,
									lEvent.mKeyScancode,
									PyBool_FromLong(lEvent.mWithShift),
									PyBool_FromLong(lEvent.mWithAlt),
									PyBool_FromLong(lEvent.mWithCtrl));
			break;
		}
		case psy::PsythonNativeEvent::KEY_UP: {
			lArglist = Py_BuildValue("(OIsiNNN)", 
									lCallbackContext, 
									lEvent.mKeyWindowUId,
									lEvent.mKeyName,
									lEvent.mKeyScancode,
									PyBool_FromLong(lEvent.mWithShift),
									PyBool_FromLong(lEvent.mWithAlt),
									PyBool_FromLong(lEvent.mWithCtrl));
			break;
		}
		default: {
			PyErr_SetString(PyExc_TypeError, "Internal error. Invalid event type.");
			return; 
		}
	}
	if (!lArglist) {
		PyErr_SetString(PyExc_TypeError, "Internal error. Callback arglist empty.");
		return;
	}
	lResult = PyObject_CallObject(lCallback, lArglist);
	Py_DECREF(lArglist);
	if (!lResult) {
		return;
	}
	Py_DECREF(lResult);
}

PyObject* registerNativeEventCallback_(int32_t inEventType, PyObject* inArgs)
{
	if (!gPsythonRendererCore) {
		PyErr_SetString(PyExc_TypeError, "PsythonRenderer has not been initialized: call PsythonRenderer.initialize() first");
		return NULL;
	}
	PyObject* lCallback = nullptr;
	PyObject* lCallbackContext = nullptr;
	if (PyArg_ParseTuple(inArgs, "OO", &lCallback, &lCallbackContext)) {
        if (!PyCallable_Check(lCallback)) {
            PyErr_SetString(PyExc_TypeError, "First parameter must be callable");
            return NULL;
        }
	}
	int32_t lId = gPsythonRendererCore->addListener(inEventType, sOnNativeEvent, static_cast<void*>(inArgs));
	if (lId == psy::PsythonEventNotifier::INVALID_LISTENER_ID) {
		assert(false);
		PyErr_SetString(PyExc_TypeError, "Internal error. Could not register callback. ");
        return NULL;
	}
	Py_INCREF(inArgs);
	return PyLong_FromLong(lId);
}

/*/////////////////////////////////*/

// Functions called directly by python

extern "C" { 

PyObject* unregisterNativeEventCallback_(PyObject* inSelf, PyObject* inArgs)
{
	if (!gPsythonRendererCore) {
		PyErr_SetString(PyExc_TypeError, "PsythonRenderer has not been initialized: call PsythonRenderer.initialize() first");
		return NULL;
	}
	int32_t lCallbackId = psy::PsythonEventNotifier::INVALID_LISTENER_ID;
	if (!PyArg_ParseTuple(inArgs, "i", &lCallbackId)) {
		PyErr_SetString(PyExc_TypeError, "Parameter must be an integer indicating the id of the callback to unregister");
		return NULL;
	}
	void* lContext = gPsythonRendererCore->getCallbackContext(lCallbackId);
	if (lContext) {
		gPsythonRendererCore->removeListener(lCallbackId);
		Py_DECREF(static_cast<PyObject*>(lContext));
	}
    Py_RETURN_NONE;
}

PyObject* registerOnQuitRequested_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::QUIT_REQUESTED, inArgs);
}

PyObject* registerOnWindowCloseRequested_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::WINDOW_CLOSE_REQUESTED, inArgs);
}

PyObject* registerOnWindowVisibilityChanged_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::WINDOW_VISIBILITY_CHANGED, inArgs);
}

PyObject* registerOnWindowFocusChanged_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::WINDOW_FOCUS_CHANGED, inArgs);
}

PyObject* registerOnWindowSizeChanged_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::WINDOW_SIZE_CHANGED, inArgs);
}

PyObject* registerOnMouseMoved_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::MOUSE_MOVED, inArgs);
}

PyObject* registerOnMouseButtonDown_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::MOUSE_DOWN, inArgs);
}

PyObject* registerOnMouseButtonUp_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::MOUSE_UP, inArgs);
}

PyObject* registerOnKeyDown_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::KEY_DOWN, inArgs);
}

PyObject* registerOnKeyUp_(PyObject*, PyObject* inArgs)
{
	return registerNativeEventCallback_(psy::PsythonNativeEvent::KEY_UP, inArgs);
}

PyObject* initialize_(PyObject*, PyObject*)
{
	if (gModuleInitializationRefCount++ == 0) {
		PySys_WriteStdout("PsythonRenderer - Initializing...\n");
		if (!gPsythonRendererCore) {
			gPsythonRendererCore.reset(new (std::nothrow) psy::PsythonRendererCore);
			if (!gPsythonRendererCore) {
				PySys_WriteStdout("PsythonRenderer - Failed to initialize, out of memory!\n");
				Py_RETURN_FALSE;
			}
			if (!gPsythonRendererCore->initialize()) {
				gPsythonRendererCore.reset();
				PySys_WriteStdout("PsythonRenderer - Failed to initialize, %s!\n", SDL_GetError());
				Py_RETURN_FALSE;
			}
		}
		PySys_WriteStdout("PsythonRenderer - Initialized\n");
	}
	Py_RETURN_TRUE;
}

void cleanup_(void* inSelf = nullptr) 
{
	if (gPsythonRendererCore) {
		// if inSelf is not null, this means the module is unloading so we don't care about the init refcount
		if (inSelf || (--gModuleInitializationRefCount == 0)) { 
			PySys_WriteStdout("PsythonRenderer - Disposing...\n");
			size_t lListenersCount = gPsythonRendererCore->getListenerCount();
			for (size_t i = 0; i < lListenersCount; ++i) {
				int32_t lListenerId = gPsythonRendererCore->getListenerIdByIndex(i);
				void* lContext = gPsythonRendererCore->getCallbackContext(lListenerId);
				if (lContext) {
					Py_DECREF(static_cast<PyObject*>(lContext));
				}
			}
			for (size_t i = 0; i < lListenersCount; ++i) {
				int32_t lListenerId = gPsythonRendererCore->getListenerIdByIndex(i);
				gPsythonRendererCore->removeListener(lListenerId);
			}
			gPsythonRendererCore.reset();
			PySys_WriteStdout("PsythonRenderer - Disposed\n");
		}
	}
	if (inSelf) {
		PyObject_Free(inSelf);
		gModuleInitializationRefCount = 0;
	}
}

PyObject* dispose_(PyObject*, PyObject*)
{
	cleanup_();
	Py_RETURN_NONE;
}

PyObject* createWindow_(PyObject*, PyObject*)
{
	if (!gPsythonRendererCore) {
		PyErr_SetString(PyExc_TypeError, "PsythonRenderer has not been initialized: call PsythonRenderer.initialize() first");
		return NULL;
	}
	uint32_t lWindowUid = gPsythonRendererCore->createWindow();
	if (lWindowUid == psy::PsythonRendererCore::INVALID_PSYTHONWINDOW_UID) {
		PyErr_SetString(PyExc_TypeError, "PsythonRenderer could not create a window: a maximum of four windows can be created at a time or you are out of memory");
		return NULL;
	}
	psy::PsythonWindowPyHandle* lWindowHandle = PyObject_New(psy::PsythonWindowPyHandle, psy::getPsythonWindowPyTypeObject());
	if (!lWindowHandle) {
		PyErr_SetString(PyExc_TypeError, "PsythonRenderer could not create a window: out of memory");
		return NULL;
	}
	lWindowHandle->mWindowUID = lWindowUid;
	lWindowHandle->mCore = gPsythonRendererCore.get();
	return (PyObject*)lWindowHandle;
}

PyObject* pumpNativeEvents_(PyObject*, PyObject*)
{
	if (!gPsythonRendererCore) {
		PyErr_SetString(PyExc_TypeError, "PsythonRenderer has not been initialized: call PsythonRenderer.initialize() first");
		return NULL;
	}
	gPsythonRendererCore->pumpNativeEvents();
	if (PyErr_Occurred()) {
		return NULL;
	}
	Py_RETURN_NONE;
}

PyMethodDef PsythonRendererMethods[] = {
	{
		"initialize",
		initialize_,
		METH_NOARGS,
		"Start-Up PsythonRenderer and acquire resources"
	},
	{
		"dispose",
		dispose_,
		METH_NOARGS,
		"Tear-down PsythonRenderer and release resources"
	},	
	{
		"createWindow",
		createWindow_,
		METH_NOARGS,
		"Create a renderable window"
	},
	{
		"pumpNativeEvents",
		pumpNativeEvents_,
		METH_NOARGS,
		"Fetches and dispatches the latests keyboard, mouse and window events"
	},
	{
		"unregisterNativeEventCallback",
		unregisterNativeEventCallback_,
		METH_VARARGS,
		"Unregisters a method previously registered with a register*() function."
	},
	{
		"registerOnQuitRequested",
		registerOnQuitRequested_,
		METH_VARARGS,
		"Registers a method called when the application requests to exit. (i.e. either all windows got closed or on OSX when cmd+Q is pressed)"
		"You are in charge of actually exiting the application if you want this behaviour. Multiple callbacks can be registered if needed."
	},
	{
		"registerOnWindowCloseRequested",
		registerOnWindowCloseRequested_,
		METH_VARARGS,
		"Registers a method called when a window created by this framework is requesting to be closed (i.e. the x button has been pressed)."
		"You are in charge of actually closing the window if you want this behaviour. Multiple callbacks can be registered if needed."
	},
	{
		"registerOnWindowVisibilityChanged",
		registerOnWindowVisibilityChanged_,
		METH_VARARGS,
		"Registers a method called when a window created by this framework is shown or hidden. Multiple callbacks can be registered if needed."
	},
	{
		"registerOnWindowFocusChanged",
		registerOnWindowFocusChanged_,
		METH_VARARGS,
		"Registers a method called when a window created by this framework obtains or losts focus. Only one window at a time can listen to the keyboard keypress events."
		"The window that actually listens to the keyboard is considered to have focus. Multiple callbacks can be registered if needed."
	},
	{
		"registerOnWindowSizeChanged",
		registerOnWindowSizeChanged_,
		METH_VARARGS,
		"Registers a method called when a window created by this framework changes size, either programatically or through the user inputs. "
		"Multiple callbacks can be registered if needed."
	},
	{
		"registerOnMouseMoved",
		registerOnMouseMoved_,
		METH_VARARGS,
		"Registers a method called when the mouse cursor position changes while over a window created by this framework."
		"Multiple callbacks can be registered if needed."
	},
	{
		"registerOnMouseButtonDown",
		registerOnMouseButtonDown_,
		METH_VARARGS,
		"Registers a method called when a button on the mouse is pressed while over a window created by this framework."
		"Multiple callbacks can be registered if needed."
	},
	{
		"registerOnMouseButtonUp",
		registerOnMouseButtonUp_,
		METH_VARARGS,
		"Registers a method called when a button on the mouse is released while over a window created by this framework."
		"Multiple callbacks can be registered if needed."
	},
	{
		"registerOnKeyDown",
		registerOnKeyDown_,
		METH_VARARGS,
		"Registers a method called when a key on the keyboard is pressed while having focus in a window created by this framework."
		"Multiple callbacks can be registered if needed."
	},
	{
		"registerOnKeyUp",
		registerOnKeyUp_,
		METH_VARARGS,
		"Registers a method called when a key on the keyboard is released while having focus in a window created by this framework."
		"Multiple callbacks can be registered if needed."
	},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

struct PyModuleDef PsythonRenderer_module_def = {
	PyModuleDef_HEAD_INIT,
	"_PsythonRenderer",
	"Internal \"_PsythonRenderer\" module",
	-1,
	PsythonRendererMethods,
	NULL,
	NULL,
	NULL,
	cleanup_
};

} // extern "C"

} // anonymous namespace

/*////////////////////////////*/

extern "C" PyMODINIT_FUNC PyInit__PsythonRenderer()
{
	if (PyType_Ready(psy::getPsythonWindowPyTypeObject()) < 0) {
        return NULL;
	}
	return PyModule_Create(&PsythonRenderer_module_def);
}