#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

std::string test ()
{
    return "Hello World!";
}

PYBIND11_MODULE(lib, handle) {
    handle.def("test", &test);
}