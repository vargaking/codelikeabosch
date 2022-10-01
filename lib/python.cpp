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

                for (int o = 0; o <= 14; o++)
                {
                    if ((int)values[3][i][o + 32] != 0)
                    {
                        MeasuredState state;
                        state.type = ObjectType((int)values[3][i][o + 32]);
                        for (int p = 0; p < 3; p++)
                        {
                            state.error[p] = INFINITE_ERROR;
                        }

                        state.position[0] = values[3][i][o + 2] / 128.0;
                        state.position[1] = values[3][i][o + 17] / 128.0;
                        state.position[2] = 0;
                        state.velocity[0] = values[3][i][o + 47] / 256.0;
                        state.velocity[1] = values[3][i][o + 62] / 256.0;
                        state.velocity[2] = 0;
                        state.acceleration[0] = 0;
                        state.acceleration[1] = 0;
                        state.acceleration[2] = 0;

                        object_states.push_back(state);
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
                        state.acceleration[0] = values[3][i][67 + t + y] / 2048.0;
                        state.acceleration[1] = values[3][i][108 + t + y] / 2048.0;
                        state.acceleration[2] = 0;
                        state.position[0] = values[3][i][149 + t + y] / 128.0;
                        state.position[1] = values[3][i][190 + t + y] / 128.0;
                        state.position[2] = values[3][i][231 + t + y] / 128.0;
                        state.velocity[0] = values[3][i][313 + t + y] / 256.0;
                        state.velocity[1] = values[3][i][354 + t + y] / 256.0;
                        state.velocity[2] = values[3][i][395 + t + y] / 256.0;
                        object_states.push_back(state);
                    }
                }
                tick.object_states = object_states;
            }
        }
        if (tick.is_host_updated)
        {
            std::cout << "Tick: " << tick.is_host_updated << std::endl;
        }

        w.tick(tick);
        std::cout << w.host.prediction.position[0] << " " << w.host.prediction.position[1] << std::endl;
    }

    return "Hello World!";
}

PYBIND11_MODULE(lib, handle)
{
    handle.def("test", &test);
    handle.def("testing", &testing);
}
