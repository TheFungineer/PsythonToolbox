from ._PsythonRenderer import initialize
from ._PsythonRenderer import dispose
from ._PsythonRenderer import createWindow
from ._PsythonRenderer import pumpNativeEvents
from ._PsythonRenderer import unregisterNativeEventCallback
from ._PsythonRenderer import registerOnQuitRequested
from ._PsythonRenderer import registerOnWindowCloseRequested
from ._PsythonRenderer import registerOnWindowVisibilityChanged
from ._PsythonRenderer import registerOnWindowFocusChanged
from ._PsythonRenderer import registerOnWindowSizeChanged
from ._PsythonRenderer import registerOnMouseMoved
from ._PsythonRenderer import registerOnMouseButtonDown
from ._PsythonRenderer import registerOnMouseButtonUp
from ._PsythonRenderer import registerOnKeyDown
from ._PsythonRenderer import registerOnKeyUp
from ._PsythonRenderer import pumpNativeEvents

import time

class PsythonRenderApp:
	def __init__(self):
		self._isrunning = False
		self._onQuitRequestedId = 2147483647
		self._onWindowCloseRequestedId = 2147483647
		self._onWindowVisibilityChangedId = 2147483647
		self._onMouseMovedId = 2147483647
		self._onWindowFocusChangedId = 2147483647
		self._onWindowSizeChangedId = 2147483647
		self._onMouseButtonDownId = 2147483647
		self._onMouseButtonUpId = 2147483647
		self._onKeyDownId = 2147483647
		self._onKeyUpId = 2147483647
		
	def registerCallbacks(self):
		self.unregisterCallbacks()
		self._onQuitRequestedId = registerOnQuitRequested(type(self).onAppExitRequested, self)
		self._onWindowCloseRequestedId = registerOnWindowCloseRequested(type(self).onWindowCloseRequested, self)
		self._onWindowVisibilityChangedId = registerOnWindowVisibilityChanged(type(self).onWindowVisibilityChanged, self)
		self._onMouseMovedId = registerOnMouseMoved(type(self).onMouseMoved, self)
		self._onWindowFocusChangedId = registerOnWindowFocusChanged(type(self).onWindowFocusChanged, self)
		self._onWindowSizeChangedId = registerOnWindowSizeChanged(type(self).onWindowSizeChanged, self)
		self._onMouseButtonDownId = registerOnMouseButtonDown(type(self).onMouseDown, self)
		self._onMouseButtonUpId = registerOnMouseButtonUp(type(self).onMouseUp, self)
		self._onKeyDownId = registerOnKeyDown(type(self).onKeyDown, self)
		self._onKeyUpId = registerOnKeyUp(type(self).onKeyUp, self)
		
	def unregisterCallbacks(self):
		unregisterNativeEventCallback(self._onQuitRequestedId)
		unregisterNativeEventCallback(self._onWindowCloseRequestedId)
		unregisterNativeEventCallback(self._onWindowVisibilityChangedId)
		unregisterNativeEventCallback(self._onMouseMovedId)
		unregisterNativeEventCallback(self._onWindowFocusChangedId)
		unregisterNativeEventCallback(self._onWindowSizeChangedId)
		unregisterNativeEventCallback(self._onMouseButtonDownId)
		unregisterNativeEventCallback(self._onMouseButtonUpId)
		unregisterNativeEventCallback(self._onKeyDownId)
		unregisterNativeEventCallback(self._onKeyUpId)
		self._onQuitRequestedId = 4294967295
		self._onWindowCloseRequestedId = 4294967295
		self._onWindowVisibilityChangedId = 4294967295
		self._onMouseMovedId = 4294967295
		self._onWindowFocusChangedId = 4294967295
		self._onWindowSizeChangedId = 4294967295
		self._onMouseButtonDownId = 4294967295
		self._onMouseButtonUpId = 4294967295
		self._onKeyDownId = 4294967295
		self._onKeyUpId = 4294967295		
		
	def run(self):
		self._isrunning = True
		initialize()
		self.registerCallbacks()
		self.onInitialize()
		while self._isrunning:
			pumpNativeEvents()
			time.sleep(0.016)
		self.unregisterCallbacks()
		dispose()
			
	def stop(self):
		self._isrunning = False
		
	def createWindow(self):
		return createWindow()
		
	def onInitialize(self):
		pass
			
	def onWindowVisibilityChanged(self, winId, isvisible):
		pass
		
	def onWindowSizeChanged(self, winId, width, height):
		pass
			
	def onWindowFocusChanged(self, winId, hasFocus):
		pass

	def onWindowCloseRequested(self, winId):
		pass
		
	def onAppExitRequested(self):
		pass
		
	def onMouseMoved(self, winId, cursorx, cursory, isRelative, downbuttons):
		pass
		
	def onMouseDown(self, winId, cursorx, cursory, downbuttons):
		pass
			
	def onMouseUp(self, winId, cursorx, cursory, downbuttons):
		pass
		
	def onKeyDown(self, winId, keyname, scancode, withshift, withalt, withctrl):
		pass
		
	def onKeyUp(self, winId, keyname, scancode, withshift, withalt, withctrl):
		pass