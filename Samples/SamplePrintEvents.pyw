##
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
##

import sys
from PsythonRenderer import PsythonRenderApp
from PsythonRenderer import EPsythonMouseButtons

class App(PsythonRenderApp):
	def onInitialize(self):
		self.registerCallbacks()
		self.win = self.createWindow()
		self.win.title = u'Test Window éàè 哈哈'
		self.win.show()
		
	def onWindowVisibilityChanged(self, winId, isvisible):
		if isvisible:
			print("window: " + str(winId) + " shown")
		else:
			print("window: " + str(winId) + " hidden")

	def onWindowSizeChanged(self, winId, width, height):
		print("window: " + str(winId) + " " + str(width) + "x" + str(height))
			
	def onWindowFocusChanged(self, winId, hasFocus):
		if hasFocus:
			print("window: " + str(winId) + " focus gain")
		else:
			print("window: " + str(winId) + " focus lost")

	def onWindowCloseRequested(self, winId):
		print("window: " + str(winId) + " closed")
		if winId == self.win.UId:
			del self.win
		
	def onAppExitRequested(self):
		print("app exiting")
		self.stop()
		
	def onMouseMoved(self, winId, cursorx, cursory, isRelative, downbuttons):
		print(str(cursorx) + " " + str(cursory) + " isrel = " + str(isRelative))
		if EPsythonMouseButtons(downbuttons) & EPsythonMouseButtons.MOUSE_BUTTON_LEFT:
			print("Left Mouse Button Down")
		if EPsythonMouseButtons(downbuttons) & EPsythonMouseButtons.MOUSE_BUTTON_MIDDLE:
			print("Middle Mouse Button Down")
		if EPsythonMouseButtons(downbuttons) & EPsythonMouseButtons.MOUSE_BUTTON_RIGHT:
			print("Right Mouse Button Down")
		if EPsythonMouseButtons(downbuttons) & EPsythonMouseButtons.MOUSE_BUTTON_X1:
			print("X1 Mouse Button Down")
		if EPsythonMouseButtons(downbuttons) & EPsythonMouseButtons.MOUSE_BUTTON_X2:
			print("X2 Mouse Button Down")
		
	def onMouseDown(self, winId, cursorx, cursory, downbuttons):
		if EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_LEFT:
			print("Left Mouse Button Down")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_MIDDLE:
			print("Middle Mouse Button Down")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_RIGHT:
			print("Right Mouse Button Down")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_X1:
			print("X1 Mouse Button Down")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_X2:
			print("X2 Mouse Button Down")
			
	def onMouseUp(self, winId, cursorx, cursory, downbuttons):
		if EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_LEFT:
			print("Left Mouse Button Up")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_MIDDLE:
			print("Middle Mouse Button Up")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_RIGHT:
			print("Right Mouse Button Up")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_X1:
			print("X1 Mouse Button Up")
		elif EPsythonMouseButtons(downbuttons) == EPsythonMouseButtons.MOUSE_BUTTON_X2:
			print("X2 Mouse Button Up")
			
	def onKeyDown(self, winId, keyname, scancode, withshift, withalt, withctrl):
		print("keydown: ", keyname, scancode, withshift, withalt, withctrl)
	
	def onKeyUp(self, winId, keyname, scancode, withshift, withalt, withctrl):
		print("keyup: ", keyname, scancode, withshift, withalt, withctrl)
		

if __name__ == "__main__":
    myapp = App()
    myapp.run()