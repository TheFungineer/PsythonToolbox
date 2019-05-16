import sys
from skbuild import setup

setup(
    name="PsythonToolbox",
    version="0.0.1",
    description="The Psychtoolbox Project, with an updated renderer.",
    author='Simon R.-Girard',
    license="MIT",
    packages=['PsythonToolbox', 'PsythonRenderer']
)