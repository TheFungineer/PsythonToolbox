import sys
from skbuild import setup

setup(
    name="PsythonToolbox",
    version="0.0.1",
    description="The Psychtoolbox Project, with an updated renderer.",
    author='Simon R.-Girard',
    license="MIT",
    packages=['PsythonToolbox', 'PsythonRenderer'],
	include_package_data=True,
	package_data={
        'PsythonRenderer': ['3rdparty/MsWinDependencies/SDL2-2.0.9/lib/x64/SDL2.dll']
    },
	#data_files=[('PsythonRenderer', (['3rdparty/MsWinDependencies/SDL2-2.0.9/lib/x64/SDL2.dll'] if sys.platform.startswith("win") else []))],
	cmake_args=[] + (
		['-DCMAKE_PREFIX_PATH=./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/lib/x64',
		'-DCMAKE_INCLUDE_PATH=./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/include'] if sys.platform.startswith("win") else []
		)
)