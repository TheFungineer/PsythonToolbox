import sys
import os
import struct
from packaging.version import LegacyVersion
from skbuild import setup
from skbuild.exceptions import SKBuildError
from skbuild.cmaker import get_cmake_version

# 64-bit only until needed
if 8 * struct.calcsize("P") != 64:
	raise SKBuildError('Sorry, Python 32 bit is not supported')

# Python must be using the vcruntime140.dll crt on MsWindows
if sys.version_info < (3,5):
    raise SKBuildError('Sorry, Python < 3.5 is not supported')

# Add CMake as a build requirement if cmake is not installed or is too low a version
setup_requires_ = []
try:
    if LegacyVersion(get_cmake_version()) < LegacyVersion("3.13"):
        setup_requires_.append('cmake')
except SKBuildError:
    setup_requires_.append('cmake')
	
cmake_args_ = []
package_data_ = {}

if os.name == 'nt':
	cmake_args_.append('-DCMAKE_PREFIX_PATH=\"./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/lib\"')
	cmake_args_.append('-DCMAKE_INCLUDE_PATH=\"./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/include\"')
	#cmake_args_.append('-DCMAKE_EXPORT_COMPILE_COMMANDS=ON')
	package_data_['PsythonRenderer'] = [
		'3rdparty/MsWinDependencies/SDL2-2.0.9/lib/SDL2.dll',
		'3rdparty/MsWinDependencies/VCRedist/msvcp140.dll']
		
if os.name == 'posix':
	if sys.platform == "darwin":
		cmake_args_.append('-DCMAKE_PREFIX_PATH=/usr/local/lib/')
		cmake_args_.append('-DCMAKE_INCLUDE_PATH=/usr/local/include/SDL2/')

setup(
    name="PsythonToolbox",
    version="0.0.1",
    description="The Psychtoolbox Project, with an updated renderer.",
    author='Simon R.-Girard',
    license="MIT",
    setup_requires=setup_requires_,
    packages=['PsythonToolbox', 'PsythonRenderer'],
	include_package_data=True,
	package_data=package_data_,
	cmake_args=cmake_args_,
	cmake_minimum_required_version="3.13.0"
)