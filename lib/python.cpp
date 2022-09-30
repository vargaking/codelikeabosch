#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

<<<<<<< HEAD
PYBIND11_MODULE(lib, handle) {
=======
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
>>>>>>> 78341d3dcdbbbece268a46e735d380fe46898043
    handle.def("test", &test);
    handle.def("testing", &testing);
}
