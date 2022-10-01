#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include "world.cpp"
#include <vector>
#include <json.hpp>

namespace py = pybind11;
using json = nlohmann::json;

#define TICKS_PER_FRAME 10

std::string testing(std::vector<std::vector<std::vector<float>>> values)
{
    std::vector<std::vector<ObjectSnapshot>> result;
    int last_frame = 0;
    int lastitem1 = 0;
    int lastitem2 = 0;
    int first = 1;

    int b = ceil(values[4][values[4].size() - 1][0] * 100.0);

    World w;

    for (int f = first; f <= 3000; f += first)
    {
        TickData tick;
        tick.is_host_updated = false;
        tick.host_state = HostMeasuredState();
        for (int p = 0; p < 3; p++)
        {
            tick.host_state.error[p] = 0;
        }

        std::vector<MeasuredState> object_states;
        if (lastitem1 < values[4].size())
        {
            int timestamp = ceil(values[4][lastitem1][0] * 100.0);
            if (timestamp == f)
            {
                tick.is_host_updated = true;
                tick.host_state.velocity = values[4][lastitem1][5] / 256.0;
                tick.host_state.acceleration = values[4][lastitem1][1] / 2048.0;
                tick.host_state.yaw_rate = values[4][lastitem1][3] / 16384.0;
                lastitem1++;
            }
        }
        if (lastitem2 < values[3].size())
        {
            int timestamp2 = ceil(values[3][lastitem2][0] * 100.0);
            if (timestamp2 <= f)
            {

                for (int o = 0; o <= 14; o++)
                {
                    if ((int)values[3][lastitem2][o + 32] != 0)
                    {
                        MeasuredState state;
                        state.type = ObjectType((int)values[3][lastitem2][o + 32]);
                        for (int p = 0; p < 3; p++)
                        {
                            if (p == 2)
                            {
                                state.error[p] = INFINITE_ERROR;
                            }
                            state.error[p] = 0;
                        }

                        state.position[0] = values[3][lastitem2][o + 2] / 128.0;
                        state.position[1] = values[3][lastitem2][o + 17] / 128.0;
                        state.position[2] = 0;
                        state.velocity[0] = values[3][lastitem2][o + 47] / 256.0;
                        state.velocity[1] = values[3][lastitem2][o + 62] / 256.0;
                        state.velocity[2] = 0;
                        state.acceleration[0] = 0;
                        state.acceleration[1] = 0;
                        state.acceleration[2] = 0;

                        tick.object_states.push_back(state);
                    }
                }
                for (int y = 0; y < 41; y += 4)
                {
                    for (int t = 0; t < 4; t++)
                    {
                        MeasuredState state;
                        state.type = ObjectType::unknown;
                        for (int p = 0; p < 3; p++)
                        {
                            state.error[p] = 0;
                        }
                        state.acceleration[0] = values[3][lastitem2][67 + t + y] / 2048.0;
                        state.acceleration[1] = values[3][lastitem2][108 + t + y] / 2048.0;
                        state.acceleration[2] = 0;
                        state.position[0] = values[3][lastitem2][149 + t + y] / 128.0;
                        state.position[1] = values[3][lastitem2][190 + t + y] / 128.0;
                        state.position[2] = values[3][lastitem2][231 + t + y] / 128.0;
                        state.velocity[0] = values[3][lastitem2][313 + t + y] / 256.0;
                        state.velocity[1] = values[3][lastitem2][354 + t + y] / 256.0;
                        state.velocity[2] = values[3][lastitem2][395 + t + y] / 256.0;
                        if (state.position[0] != 0 || state.position[0] != 0)
                        {
                            tick.object_states.push_back(state);
                        }
                    }
                }
                lastitem2++;
            }
        }
        if (tick.is_host_updated)
        {
            // std::cout << "Tick: " << tick.is_host_updated << std::endl;
        }

        w.tick(tick);
        last_frame++;
        if (last_frame == TICKS_PER_FRAME)
        {
            result.push_back(w.export_objects());
            last_frame = 0;
        }
        // std::cout << w.time << " " << w.host.prediction.x << " " << w.host.prediction.y << std::endl;
        // std::cout << w.objects.size() << std::endl;
        std::cout << lastitem2 << " " << values[3].size() << "\n";
    }

    std::string outputstring = "[";
    for (int x = 0; x < result.size(); x++)
    {
        outputstring += "[";
        for (int p = 0; p < result[x].size(); p++)
        {

            json j2 = {
                {"x", result[x][p].x},
                {"y", result[x][p].y},
                {"type", result[x][p].type},
                {"id", result[x][p].id},
            };
            std::string jfa = to_string(j2);
            outputstring += jfa;
            outputstring += ",";
        }
        if (x != result.size() - 1)
        {
            outputstring += "],";
        }
        else
        {
            outputstring += "]";
        }
    }
    outputstring += "]";

    return outputstring;
}

PYBIND11_MODULE(lib, handle)
{
    handle.def("testing", &testing);

    py::class_<ObjectSnapshot>(handle, "ObjectSnapshot")
        .def("get_type", &ObjectSnapshot::get_type)
        .def("get_x", &ObjectSnapshot::get_x)
        .def("get_y", &ObjectSnapshot::get_y)
        .def("get_z", &ObjectSnapshot::get_z)
        .def("get_id", &ObjectSnapshot::get_id);
}