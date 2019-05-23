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
	
from ._PsythonRenderApp import PsythonRenderApp
from ._PsythonMouseButtons import PsythonMouseButtons