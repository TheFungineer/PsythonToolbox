import ctypes
import os

if os.name == 'nt':
	user32 = ctypes.windll.user32 
	user32.SetProcessDPIAware()
	msWinSdlDllPath = os.path.dirname(__file__)
	ctypes.cdll.LoadLibrary(msWinSdlDllPath + "\\3rdparty\\MsWinDependencies\\SDL2-2.0.9\\lib\\SDL2.dll")
	try:
		ctypes.cdll.LoadLibrary("msvcp140.dll")
	except:
		ctypes.cdll.LoadLibrary(msWinSdlDllPath + "\\3rdparty\\MsWinDependencies\\VCRedist\\msvcp140.dll")
	
if os.name == 'posix' :
	if sys.platform == "darwin":
		osxSdlDllPath = "/usr/local/lib/"
		ctypes.cdll.LoadLibrary(osxSdlDllPath + "libsdl2.dylib")
	
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
from ._PsythonMouseButtons import PsythonMouseButtons