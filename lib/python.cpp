#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "world.cpp"
#include <vector>

namespace py = pybind11;

#define length size()

std::string test()
{
    return "Hello World!";
}

std::string testing(std::vector<std::vector<std::vector<float>>> values)
{

    float first = 0.01;
    float b = ceil(values[4][values[4].size() - 1][0] * 100.0) / 100.0;
    World w;

    for (float f = first; f <= b; f += first)
    {
        TickData tick;
        tick.is_host_updated = false;
        tick.host_state = HostMeasuredState();
        for (int p = 0; p < 3; p++)
        {
            tick.host_state.error[p] = 0.1;
        }
        std::vector<MeasuredState> object_states;
        tick.object_states = object_states;

        for (int i = 0; i < values[4].size(); i++)
        {
            float timestamp = ceil(values[4][i][0] * 100.0);
            if (timestamp == int(f * 100))
            {
                tick.is_host_updated = true;
            }
        }
        for (int i = 0; i < values[3].size(); i++)
        {
            int timestamp = ceil(values[3][i][0] * 100.0);
            if (timestamp == int(f * 100))
            {
                std::vector<MeasuredState> object_states;
                for (int o = 32; o <= 46; o++)
                {
                    MeasuredState state;
                    state.type = ObjectType((int)values[3][i][o]);
                    for (int p = 0; p < 3; p++)
                    {
                        state.error[p] = 0.1;
                    }
                    object_states.push_back(state);
                }
                tick.object_states = object_states;
            }
        }
        if (tick.is_host_updated)
        {
            std::cout << "Tick: " << tick.is_host_updated << std::endl;
        }

        w.tick(tick);
    }

    return "Hello World!";
}

PYBIND11_MODULE(lib, handle)
{
    handle.def("test", &test);
    handle.def("testing", &testing);
}
