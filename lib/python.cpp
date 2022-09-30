#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "world.hpp"

namespace py = pybind11;

std::string test()
{
    return "Hello World!";
}

std::string testing()
{
    return "Hello World222!";
}

PYBIND11_MODULE(lib, handle)
{
    handle.def("test", &test);
    handle.def("testing", &testing);
}
