from setuptools import setup

# pyproject.toml allows us to use this at start time
from pybind11.setup_helpers import Pybind11Extension, build_ext


__version__ = "0.0.1"
import os
os.environ["CC"] = "g++"





ext_modules = [
    Pybind11Extension("celeste",
    sources = ["gym_celex/envs/module.cpp","gym_celex/envs/celeste.c"],
    libraries=['SDL2','SDL2_mixer','python3.8','m'],
    extra_compile_args=['-fpermissive']
    )

]

setup(name = 'gym_celex',
    version=__version__,
    ext_modules=ext_modules,
    install_requires=['gym>=0.18.3',
        'numpy>=1.21.1']

)