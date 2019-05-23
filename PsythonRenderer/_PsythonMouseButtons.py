from enum import IntFlag

class PsythonMouseButtons(IntFlag):
	MOUSE_NO_BUTTONS = 0
	MOUSE_BUTTON_LEFT = 1 << 0
	MOUSE_BUTTON_MIDDLE = 1 << 1
	MOUSE_BUTTON_RIGHT = 1 << 2
	MOUSE_BUTTON_X1 = 1 << 3
	MOUSE_BUTTON_X2 = 1 << 4