#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "world.cpp"
#include <vector>

namespace py = pybind11;

std::string test()
{
    return "Hello World!";
}

std::string testing()
{
    TickData tickData;
    tickData.is_host_updated = false;
    tickData.host_state = MeasuredState();
    tickData.host_state.type = ObjectType::car;
    for (int i = 0; i < 3; i++)
    {
        tickData.host_state.error[i] = 0;
    }
    std::vector<MeasuredState> object_states;
    object_states.push_back(MeasuredState());
    object_states[0].type = ObjectType::car;
    for (int i = 0; i < 3; i++)
    {
        object_states[0].error[i] = 0;
    }
    tickData.object_states = object_states;
    tickData.timestamp = 1231.124;
    return std::to_string(tickData.timestamp);
}

PYBIND11_MODULE(lib, handle)
{
    handle.def("test", &test);
    handle.def("testing", &testing);
}
