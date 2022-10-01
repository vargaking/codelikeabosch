#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "world.cpp"
#include <vector>

namespace py = pybind11;

#define length size()

#define TICKS_PER_FRAME 5

std::string test()
{
    return "Hello World!";
}

std::vector<std::vector<ObjectSnapshot>> testing(std::vector<std::vector<std::vector<float>>> values)
{
    std::vector<std::vector<ObjectSnapshot>> result;
    int last_frame = 0;
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
            tick.host_state.error[p] = 0;
        }

        std::vector<MeasuredState> object_states;
        tick.object_states = object_states;

        for (int i = 0; i < values[4].size(); i++)
        {
            float timestamp = ceil(values[4][i][0] * 100.0);
            if (timestamp == int(f * 100))
            {
                tick.is_host_updated = true;
                tick.host_state.velocity[0] = values[4][i][5] / 256.0;
                tick.host_state.velocity[1] = values[4][i][6] / 256.0;
                tick.host_state.acceleration[0] = values[4][i][1] / 2048.0;
                tick.host_state.acceleration[1] = values[4][i][2] / 2048.0;
                tick.host_state.yaw_rate = values[4][i][3] / 16384.0;
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
                        state.error[p] = 0;
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
        last_frame++;
        if (last_frame == TICKS_PER_FRAME) {
            result.push_back(w.export_objects());
            last_frame = 0;
        }
        std::cout << w.host.prediction.position[0] << " " << w.host.prediction.position[1] << std::endl;
    }

    return result;
}

PYBIND11_MODULE(lib, handle)
{
    handle.def("test", &test);
    handle.def("testing", &testing);

    py::class_<ObjectSnapshot>(handle, "ObjectSnapshot")
        .def("get_type", &ObjectSnapshot::get_type)
        .def("get_x", &ObjectSnapshot::get_x)
        .def("get_y", &ObjectSnapshot::get_y)
        .def("get_z", &ObjectSnapshot::get_z)
        .def("get_id", &ObjectSnapshot::get_id);
}
