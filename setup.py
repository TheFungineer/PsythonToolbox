##
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
##

import sys
import os
import struct
from skbuild import setup
from skbuild.exceptions import SKBuildError

# 64-bit only until needed
if 8 * struct.calcsize("P") != 64:
	raise SKBuildError('Sorry, Python 32 bit is not supported')
	
cmake_args_ = []

if os.name == 'nt':
	cmake_args_.append('-DCMAKE_PREFIX_PATH=\"./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/lib\"')
	cmake_args_.append('-DCMAKE_INCLUDE_PATH=\"./PsythonRenderer/3rdparty/MsWinDependencies/SDL2-2.0.9/include\"')
		
if os.name == 'posix':
	if sys.platform == "darwin":
		cmake_args_.append('-DCMAKE_PREFIX_PATH=/usr/local/lib/')
		cmake_args_.append('-DCMAKE_INCLUDE_PATH=/usr/local/include/SDL2/')

setup(
    name='PsythonToolbox',
    version='0.0.1',
    description="Inspired by the Psychtoolbox Project, but with an updated renderer",
	long_description="UI framework for scientific experimentation and visualization inspired by the Psychtoolbox project, but with an updated renderer",
    author='Simon R.-Girard',
	author_email='simon.rivard-girard [at[ polymtl dot-com',
    license='MIT',
    setup_requires=["setuptools", "wheel", "scikit-build", "ninja", 'cmake'],
    packages=['PsythonToolbox', 'PsythonRenderer'],
	include_package_data=True,
	cmake_args=cmake_args_,
	cmake_minimum_required_version='3.13.0',
	python_requires='>=3.5, <4',  # Python should be using the vcruntime140.dll crt on MsWindows
	platforms=['Windows 10', 'Windows 8.1', 'Windows 8', 'Windows 7', 'OSX', 'Linux', 'x64'],
	url='https://github.com/TheFungineer/PsythonToolbox',
    keywords='UI framework for scientific experimentation and visualization',
    classifiers=[
        'Development Status :: 1 - Planning',
		"Environment :: Win32 (MS Windows)",
        "Environment :: X11 Applications",
		"Environment :: MacOS X"
        'Intended Audience :: Education',
		'Intended Audience :: Science/Research',
        'Natural Language :: English',
        'License :: OSI Approved :: MIT License',
		'Operating System :: MacOS',
		'Operating System :: MacOS :: MacOS X',
		'Operating System :: POSIX',
		'Operating System :: POSIX :: Linux',
		'Operating System :: Microsoft',
		'Operating System :: Microsoft :: Windows',
		'Operating System :: Microsoft :: Windows :: Windows 10',
		'Operating System :: Microsoft :: Windows :: Windows 7',
		'Operating System :: Microsoft :: Windows :: Windows 8',
		'Operating System :: Microsoft :: Windows :: Windows 8.1',
        'Programming Language :: Python',
		'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
		'Programming Language :: Python :: 3.7',
		'Programming Language :: Python :: 3 :: Only',
		'Programming Language :: Python :: Implementation',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy',
		'Programming Language :: C++',
		'Programming Language :: C',
        'Topic :: Education',
		'Topic :: Multimedia',			 
		'Topic :: Multimedia :: Graphics',
		'Topic :: Multimedia :: Sound/Audio',
		'Topic :: Scientific/Engineering',
		'Topic :: Scientific/Engineering :: Human Machine Interfaces',
		'Topic :: Scientific/Engineering :: Visualization',
    ],
)