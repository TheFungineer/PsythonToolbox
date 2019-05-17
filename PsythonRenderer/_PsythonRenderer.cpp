#include <Python.h>
#include <cstdio>
#define SDL_MAIN_HANDLED
#include <SDL.h>

namespace {
	
PyObject* initialize_(PyObject *self, PyObject *args)
{
	PySys_WriteStdout("PsythonRenderer - Initializing...\n");
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		PySys_WriteStdout("PsythonRenderer - Failed to initialize, %s!\n", SDL_GetError());
		Py_RETURN_FALSE;
	}
	PySys_WriteStdout("PsythonRenderer - Initialized\n");
	Py_RETURN_TRUE;
}

PyObject* dispose_(PyObject *self, PyObject *args)
{
	PySys_WriteStdout("PsythonRenderer - Disposing...\n");
	SDL_Quit();
	PySys_WriteStdout("PsythonRenderer - Disposed\n");
	Py_RETURN_NONE;
}

PyMethodDef PsythonRendererMethods[] = {
	{
		"initialize",
		initialize_,
		METH_VARARGS,
		"Start-Up PsythonRenderer and acquire resources"
	},

	{
		"dispose",
		dispose_,
		METH_VARARGS,
		"Tear-down PsythonRenderer and release resources"
	},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

struct PyModuleDef PsythonRenderer_module_def = {
	PyModuleDef_HEAD_INIT,
	"_PsythonRenderer",
	"Internal \"_PsythonRenderer\" module",
	-1,
	PsythonRendererMethods
};

// TODO:
//	-Mouse & Keyboard entities, or all in PsythonRenderer
//	-window entity & RenderDevice -> [chooseGpu()?], setVsync, present()
//	-pumpNativeEvents
//	-un/registerNativeEventsHandlers

} // anonymous namespace

/*////////////////////////////*/

PyMODINIT_FUNC PyInit__PsythonRenderer(void)
{
	return PyModule_Create(&PsythonRenderer_module_def);
}