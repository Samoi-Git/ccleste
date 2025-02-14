#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "celexplore.cpp"
#include <iostream>
#include <pybind11/pybind11.h> //binding library
namespace py = pybind11; //command nessecitated by pybind
PYBIND11_MODULE(celeste, m) {
    m.doc() = "python bindings for celexplore"; //docstring
    py::class_<CelesteGame>(m,"CelesteGame")
        .def(py::init<>())
        .def("nextFrame",&CelesteGame::nextFrame,py::arg("up") = false,py::arg("down") = false,py::arg("left") = false,py::arg("right") = false,py::arg("dash") = false,py::arg("jump") = false)
        .def("isRunning",&CelesteGame::isRunning)
        .def("getLevel",&CelesteGame::getLevel)
        .def("renderScreen",&CelesteGame::renderScreen)
        .def("getPixel",&CelesteGame::getPixel);
}

