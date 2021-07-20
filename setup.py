from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir
import sys
ext_modules = [
    ["module.cpp"]
]
setup(
    name="celexplore",
    ext_modules=ext_modules,
)
