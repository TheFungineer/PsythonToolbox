##
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
##

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
	INVALID_CALLBACK_ID = 2147483647 	# UINT32 MAX
	def __init__(self):
		self._isrunning = False
		self._onQuitRequestedId = self.INVALID_CALLBACK_ID
		self._onWindowCloseRequestedId = self.INVALID_CALLBACK_ID
		self._onWindowVisibilityChangedId = self.INVALID_CALLBACK_ID
		self._onMouseMovedId = self.INVALID_CALLBACK_ID
		self._onWindowFocusChangedId = self.INVALID_CALLBACK_ID
		self._onWindowSizeChangedId = self.INVALID_CALLBACK_ID
		self._onMouseButtonDownId = self.INVALID_CALLBACK_ID
		self._onMouseButtonUpId = self.INVALID_CALLBACK_ID
		self._onKeyDownId = self.INVALID_CALLBACK_ID
		self._onKeyUpId = self.INVALID_CALLBACK_ID
		
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
		self._onQuitRequestedId = self.INVALID_CALLBACK_ID
		self._onWindowCloseRequestedId = self.INVALID_CALLBACK_ID
		self._onWindowVisibilityChangedId = self.INVALID_CALLBACK_ID
		self._onMouseMovedId = self.INVALID_CALLBACK_ID
		self._onWindowFocusChangedId = self.INVALID_CALLBACK_ID
		self._onWindowSizeChangedId = self.INVALID_CALLBACK_ID
		self._onMouseButtonDownId = self.INVALID_CALLBACK_ID
		self._onMouseButtonUpId = self.INVALID_CALLBACK_ID
		self._onKeyDownId = self.INVALID_CALLBACK_ID
		self._onKeyUpId = self.INVALID_CALLBACK_ID		
		
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