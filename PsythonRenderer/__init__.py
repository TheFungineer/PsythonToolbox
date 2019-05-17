import ctypes
import os

if os.name == 'nt':
	msWinSdlDllPath = os.path.dirname(__file__)
	ctypes.windll.LoadLibrary(msWinSdlDllPath + "\\3rdparty\\MsWinDependencies\\SDL2-2.0.9\\lib\\x64\\SDL2.dll")
else :
	ctypes.cdll.LoadLibrary("libsdl2.so")
	
from ._PsythonRenderer import initialize
from ._PsythonRenderer import dispose