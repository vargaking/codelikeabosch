#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>

#define INFINITE_ERROR 1000000
#define MERGE_DISTANCE 1

enum class ObjectType
{
    unknown = 0,
    truck = 1,
    car = 2,
    motorbike = 3,
    bicycle = 4,
    pedestrian = 5,
    car_or_truck = 6
};

class MotionState
{
public:
    double position[3], velocity[3], acceleration[3];
};

class MeasuredState : public MotionState
{
public:
    ObjectType type;
    double error[3];
};

class EstimateState : public MotionState
{
public:
    double gain[3], error[3];
};

class PredictionState : public MotionState
{
public:
    double error[3];
};

class Object
{
public:
    ObjectType type;
    MeasuredState measurement;
    EstimateState estimate;
    PredictionState prediction;
    int id;

    Object(MeasuredState initial_state)
    {
        for (int i = 0; i < 3; i++)
        {
            estimate.error[i] = measurement.error[i];
        }

        for (int axis = 0; axis < 3; axis++)
        {
            estimate.position[axis] = initial_state.position[axis];
            estimate.velocity[axis] = initial_state.velocity[axis];
            estimate.acceleration[axis] = initial_state.acceleration[axis];
        }

        std::random_device dev;
        std::mt19937 rng(dev());
        id = rng();
    }

    void update()
    {
        if (measurement.type != ObjectType::unknown)
        {
            type = measurement.type;
        }

        for (int i = 0; i < 3; i++)
        {
            estimate.gain[i] = prediction.error[i] / (prediction.error[i] + measurement.error[i]);
            estimate.error[i] = (1 - estimate.gain[i]) * prediction.error[i];
        }

        for (int axis = 0; axis < 3; axis++)
        {
            estimate.position[axis] = prediction.position[axis] + estimate.gain[0] * (measurement.position[axis] - prediction.position[axis]);
            estimate.velocity[axis] = prediction.velocity[axis] + estimate.gain[1] * (measurement.velocity[axis] - prediction.velocity[axis]);
            estimate.acceleration[axis] = prediction.acceleration[axis] + estimate.gain[2] * (measurement.acceleration[axis] - prediction.acceleration[axis]);
        }
    }

    void predict(double dt, double process_noise[3])
    {
        for (int i = 0; i < 3; i++)
        {
            prediction.error[i] = estimate.error[i] + process_noise[i];
        }

        for (int axis = 0; axis < 3; axis++)
        {
            prediction.acceleration[axis] = estimate.acceleration[axis];
            prediction.velocity[axis] = estimate.velocity[axis] + (estimate.acceleration[axis] * dt);
            prediction.position[axis] = estimate.position[axis] + (estimate.velocity[axis] * dt) + (0.5 * estimate.acceleration[axis] * std::pow(dt, 2));
        }
    }
};

class HostMotionState
{
public:
    double velocity[2], acceleration[2], yaw_rate;
};

class HostMeasuredState : public HostMotionState
{
public:
    double error[3];
};

class HostEstimateState : public HostMotionState
{
public:
    double position[2], gain[3], yaw_position, error[3];
};

class HostPredictionState : public HostMotionState
{
public:
    double position[2], yaw_position, error[3];
};

class HostObject
{
public:
    HostMeasuredState measurement;
    HostEstimateState estimate;
    HostPredictionState prediction;

    HostObject() = default;

    HostObject(HostMeasuredState initial_state)
    {
        for (int i = 0; i < 3; i++)
        {
            estimate.error[i] = measurement.error[i];
        }

        for (int axis = 0; axis < 2; axis++)
        {
            estimate.position[axis] = 0;
            estimate.velocity[axis] = initial_state.velocity[axis];
            estimate.acceleration[axis] = initial_state.acceleration[axis];
        }

        estimate.yaw_position = 0;
        estimate.yaw_rate = initial_state.yaw_rate;
    }

    void update()
    {
        for (int i = 0; i < 3; i++)
        {
            estimate.gain[i] = prediction.error[i] / (prediction.error[i] + measurement.error[i]);
            estimate.error[i] = (1 - estimate.gain[i]) * prediction.error[i];
        }

        for (int axis = 0; axis < 2; axis++)
        {
            estimate.position[axis] = prediction.position[axis];
            estimate.velocity[axis] = prediction.velocity[axis] + estimate.gain[0] * (measurement.velocity[axis] - prediction.velocity[axis]);
            estimate.acceleration[axis] = prediction.acceleration[axis] + estimate.gain[1] * (measurement.acceleration[axis] - prediction.acceleration[axis]);
        }

        estimate.yaw_position = prediction.yaw_position;
        estimate.yaw_rate = prediction.yaw_rate + estimate.gain[2] * (measurement.yaw_rate - prediction.yaw_rate);
    }

    void predict(double dt, double process_noise[2])
    {
        for (int i = 0; i < 3; i++)
        {
            prediction.error[i] = estimate.error[i] + process_noise[i];
        }

        for (int axis = 0; axis < 2; axis++)
        {
            prediction.acceleration[axis] = estimate.acceleration[axis];
            prediction.velocity[axis] = estimate.velocity[axis] + (estimate.acceleration[axis] * dt);
            prediction.position[axis] = estimate.position[axis] + (estimate.velocity[axis] * dt) + (0.5 * estimate.acceleration[axis] * std::pow(dt, 2));
        }

        prediction.yaw_rate = estimate.yaw_rate;
        prediction.yaw_position = estimate.yaw_position + (estimate.yaw_rate * dt);
    }
};

class TickData
{
public:
    bool is_host_updated;
    HostMeasuredState host_state;
    std::vector<MeasuredState> object_states;

    TickData()
    {
        is_host_updated = false;
    }
};

class ObjectSnapshot
{
public:
    std::string type;
    double x, y, z;
    int id;

    ObjectSnapshot(HostObject &host, Object &object)
    {
        switch (object.type)
        {
        case ObjectType::unknown:
        {
            type = "unknown";
            break;
        }
        case ObjectType::truck:
        {
            type = "bicycle";
            break;
        }
        case ObjectType::car:
        {
            type = "car";
            break;
        }
        case ObjectType::motorbike:
        {
            type = "motorbike";
            break;
        }
        case ObjectType::bicycle:
        {
            type = "bicycle";
            break;
        }
        case ObjectType::pedestrian:
        {
            type = "pedestrian";
            break;
        }
        case ObjectType::car_or_truck:
        {
            type = "car_or_truck";
            break;
        }
        }
        x = object.prediction.position[0] - host.prediction.position[0];
        y = object.prediction.position[1] - host.prediction.position[1];
        z = object.prediction.position[2];

        id = object.id;
    }

    std::string get_type() { return type; }
    double get_x() { return x; }
    double get_y() { return y; }
    double get_z() { return z; }
    int get_id() { return id; }
};

class World
{
public:
    HostObject host;
    bool host_ready;
    std::vector<Object> objects;
    double time;

public:
    World()
    {
        time = 0;
        host_ready = false;
    }

    void update_objects(std::vector<MeasuredState> sensor_data)
    {
        if (!host_ready)
            return;

        for (auto &data : sensor_data)
        {
            for (int axis = 0; axis < 2; axis++)
            {
                data.position[axis] += host.estimate.position[axis];
                data.velocity[axis] += host.estimate.velocity[axis];
                data.acceleration[axis] += host.estimate.acceleration[axis];
            }
        }

        // trying to merge new measurements into existing objects
        // if an object does not have a match, it is most likely in a blind spot
        // if a measurement does not have a match, it is most likely a new object
        std::vector<bool> object_has_match(objects.size(), false), data_has_match(sensor_data.size(), false);

        for (int i = 0; i < objects.size(); i++)
        {
            for (int j = 0; j < sensor_data.size(); j++)
            {
                if (object_has_match[i] || data_has_match[j])
                {
                    continue;
                }

                bool match = true;
                for (int axis = 0; axis < 3; axis++)
                {
                    int object_pos = objects[i].prediction.position[axis];
                    int new_pos = sensor_data[j].position[axis];
                    int difference = std::abs(new_pos - object_pos);
                    if (difference > MERGE_DISTANCE)
                    {
                        match = false;
                        break;
                    }
                }

                if (match)
                {
                    object_has_match[i] = true;
                    data_has_match[j] = true;
                    objects[i].measurement = sensor_data[j];
                }
            }
        }

        for (int i = 0; i < objects.size(); i++)
        {
            if (!object_has_match[i])
            {
                for (auto &u : objects[i].measurement.error)
                {
                    u = INFINITE_ERROR;
                }
            }

            objects[i].update();
        }

        for (int i = 0; i < sensor_data.size(); i++)
        {
            if (!data_has_match[i])
            {
                objects.push_back(Object(sensor_data[i]));
            }
        }
    }

    void tick(const TickData &data)
    {
        if (!host_ready)
        {
            if (data.is_host_updated)
            {
                host = HostObject(data.host_state);
                host_ready = true;
                double process_noise[3] = {1, 1, 1};
                host.predict(0.01, process_noise);
            }
        }
        else
        {
            if (data.is_host_updated)
            {
                host.measurement = data.host_state;
                host.update();
            }
            else
            {
                HostMeasuredState h;
                for (auto &i : h.error)
                {
                    i = INFINITE_ERROR;
                }
                host.measurement = h;
                host.update();
            }

            double process_noise[3] = {1, 1, 1};

            host.predict(0.01, process_noise);

            update_objects(data.object_states);

            for (auto &object : objects)
            {
                object.predict(0.01, process_noise);
            }
        }

        time = time + 0.01;
    }

    std::vector<ObjectSnapshot> export_objects()
    {
        std::vector<ObjectSnapshot> result;
        for (auto &object : objects)
        {
            result.push_back(ObjectSnapshot(host, object));
        }
        return result;
    }
};