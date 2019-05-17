#include <Python.h>
#include <cstdio>
#define SDL_MAIN_HANDLED
#include <SDL.h>

//-----------------------------------------------------------------------------
static PyObject* initialize(PyObject *self, PyObject *args)
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

//-----------------------------------------------------------------------------
static PyObject* dispose(PyObject *self, PyObject *args)
{
	PySys_WriteStdout("PsythonRenderer - Disposing...\n");
	SDL_Quit();
	PySys_WriteStdout("PsythonRenderer - Disposed\n");
	Py_RETURN_NONE;
}

//-----------------------------------------------------------------------------
static PyMethodDef PsythonRenderer_methods[] = {
	{
		"initialize",
		initialize,
		METH_VARARGS,
		"Start-Up PsythonRenderer and acquire resources"
	},

	{
		"dispose",
		dispose,
		METH_VARARGS,
		"Tear-down PsythonRenderer and release resources"
	},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

//-----------------------------------------------------------------------------
static struct PyModuleDef PsythonRenderer_module_def = {
	PyModuleDef_HEAD_INIT,
	"_PsythonRenderer",
	"Internal \"_PsythonRenderer\" module",
	-1,
	PsythonRenderer_methods
};

PyMODINIT_FUNC PyInit__PsythonRenderer(void)
{
	return PyModule_Create(&PsythonRenderer_module_def);
}