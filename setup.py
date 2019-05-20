import sys
from skbuild import setup

from packaging.version import LegacyVersion
from skbuild.exceptions import SKBuildError
from skbuild.cmaker import get_cmake_version

# Add CMake as a build requirement if cmake is not installed or is too low a version
setup_requires = []
try:
    if LegacyVersion(get_cmake_version()) < LegacyVersion("3.4"):
        setup_requires.append('cmake')
except SKBuildError:
    setup_requires.append('cmake')

setup(
    name="PsythonToolbox",
    version="0.0.1",
    description="The Psychtoolbox Project, with an updated renderer.",
    author='Simon R.-Girard',
    license="MIT",
    setup_requires=setup_requires,
    packages=['PsythonToolbox', 'PsythonRenderer'],
	include_package_data=True,
	package_data={
        'PsythonRenderer': (['3rdparty/MsWinDependencies/SDL2-2.0.9/lib/x64/SDL2.dll'] if sys.platform.startswith("win") else [])
    },
	#data_files=[('PsythonRenderer', (['3rdparty/MsWinDependencies/SDL2-2.0.9/lib/x64/SDL2.dll'] if sys.platform.startswith("win") else []))],
	cmake_args=[] + (
		['-DCMAKE_PREFIX_PATH=./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/lib/x64',
		'-DCMAKE_INCLUDE_PATH=./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/include'] if sys.platform.startswith("win") else []
        ) + (
            ['-DCMAKE_PREFIX_PATH=/usr/local/lib/',
            '-DCMAKE_INCLUDE_PATH=/usr/local/include/SDL2/'] if sys.platform.startswith("darwin") else []
        )
)