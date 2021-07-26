// cppimport
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "celexplore.cpp"
#include <iostream>
#include <pybind11/pybind11.h> //binding library
namespace py = pybind11; //command nessecitated by pybind
PYBIND11_MODULE(module, m) {
    m.doc() = "python bindings for celexplore"; //docstring
    py::class_<CelesteGame>(m,"CelesteGame")
        .def(py::init<>())
        .def("nextFrame",&CelesteGame::nextFrame)
        .def("isRunning",&CelesteGame::isRunning)
        .def("getPixel",&CelesteGame::getPixel);

}

/*
<%
cfg['extra_compile_args'] = ['-fpermissive']
cfg['dependencies'] = ['celexplore.cpp','sdl12main.c']
cfg['libraries'] = ['SDL2','SDL2_mixer','python3.9','m']
cfg['include_dirs'] = ['/usr/include/SDL2']
cfg['sources'] = ['celeste.c']
setup_pybind11(cfg)
%>
*/

