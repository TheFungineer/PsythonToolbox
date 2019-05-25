##########################################################
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
##########################################################

import sys
import os
import struct
from skbuild import setup
from skbuild.exceptions import SKBuildError

# 64-bit only until needed
if 8 * struct.calcsize("P") != 64:
	raise SKBuildError('Sorry, Python 32 bit is not supported')

# Python must be using the vcruntime140.dll crt on MsWindows
if sys.version_info < (3,5):
    raise SKBuildError('Sorry, Python < 3.5 is not supported')
	
cmake_args_ = []

if os.name == 'nt':
	cmake_args_.append('-DCMAKE_PREFIX_PATH=\"./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/lib\"')
	cmake_args_.append('-DCMAKE_INCLUDE_PATH=\"./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/include\"')
		
if os.name == 'posix':
	if sys.platform == "darwin":
		cmake_args_.append('-DCMAKE_PREFIX_PATH=/usr/local/lib/')
		cmake_args_.append('-DCMAKE_INCLUDE_PATH=/usr/local/include/SDL2/')

setup(
    name="PsythonToolbox",
    version="0.0.1",
    description="Inspired by the Psychtoolbox Project, but with an updated renderer.",
    author='Simon R.-Girard',
	author_email='simon.rivard-girard [at[ polymtl dot-com',
    license="MIT",
    setup_requires=["setuptools", "wheel", "scikit-build", "ninja", 'cmake'],
    packages=['PsythonToolbox', 'PsythonRenderer'],
	include_package_data=True,
	cmake_args=cmake_args_,
	cmake_minimum_required_version="3.13.0"
)