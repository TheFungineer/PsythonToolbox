import ctypes
import os

if os.name == 'nt':
	msWinSdlDllPath = os.path.dirname(__file__)
	ctypes.windll.LoadLibrary(msWinSdlDllPath + "\\3rdparty\\MsWinDependencies\\SDL2-2.0.9\\lib\\x64\\SDL2.dll")
else :
	osxSdlDllPath = "/usr/local/lib/"
	ctypes.cdll.LoadLibrary(osxSdlDllPath + "libsdl2.dylib")
	
from ._PsythonRenderer import initialize
from ._PsythonRenderer import dispose