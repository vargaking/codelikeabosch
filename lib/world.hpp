#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>

#define INFINITE_ERROR 1000000
#define MERGE_DISTANCE 3
#define TIMEOUT_TICKS 500

const double pi = 3.14159265358979;

<<<<<<< HEAD
inline double to_radian (double &degrees)
=======
inline double to_radian(double degrees)
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce
{
    return (pi * degrees) / 180;
}

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
<<<<<<< HEAD
    public:
        ObjectType type;
        MeasuredState measurement;
        EstimateState estimate;
        PredictionState prediction;
        int id;
        int timeout;

        Object (MeasuredState &initial_state)
=======
public:
    ObjectType type;
    MeasuredState measurement;
    EstimateState estimate;
    PredictionState prediction;
    int id;
    int timeout;

    Object(MeasuredState initial_state)
    {
        for (int i = 0; i < 3; i++)
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce
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

        timeout = 0;
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
    double velocity, acceleration, yaw_rate;
};

class HostMeasuredState : public HostMotionState
{
public:
    double error[3];
};

class HostEstimateState : public HostMotionState
{
public:
    double x, y, vx, vy, ax, ay, gain[3], yaw_position, error[3];
};

class HostPredictionState : public HostMotionState
{
public:
    double x, y, vx, vy, ax, ay, yaw_position, error[3];
};

class HostObject
{
public:
    HostMeasuredState measurement;
    HostEstimateState estimate;
    HostPredictionState prediction;

    HostObject() = default;

<<<<<<< HEAD
        HostObject (HostMeasuredState &initial_state)
=======
    HostObject(HostMeasuredState initial_state)
    {
        for (int i = 0; i < 3; i++)
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce
        {
            estimate.error[i] = measurement.error[i];
        }

        estimate.velocity = initial_state.velocity;
        estimate.acceleration = initial_state.acceleration;
        estimate.yaw_rate = initial_state.yaw_rate;

        estimate.yaw_position = 0;
        estimate.x = 0;
        estimate.y = 0;
        estimate.vx = 0;
        estimate.vy = 0;
        estimate.ax = 0;
        estimate.ay = 0;
    }

    void update()
    {
        for (int i = 0; i < 3; i++)
        {
            estimate.gain[i] = prediction.error[i] / (prediction.error[i] + measurement.error[i]);
            estimate.error[i] = (1 - estimate.gain[i]) * prediction.error[i];
        }

        estimate.velocity = prediction.velocity + estimate.gain[0] * (measurement.velocity - prediction.velocity);
        estimate.acceleration = prediction.acceleration + estimate.gain[1] * (measurement.acceleration - prediction.acceleration);
        estimate.yaw_rate = prediction.yaw_rate + estimate.gain[2] * (measurement.yaw_rate - prediction.yaw_rate);

        estimate.yaw_position = prediction.yaw_position;
        estimate.x = prediction.x;
        estimate.y = prediction.y;
        estimate.vx = prediction.vx;
        estimate.vy = prediction.vy;
        estimate.ax = prediction.ax;
        estimate.ay = prediction.ay;
    }

    void predict(double dt, double process_noise[2])
    {
        for (int i = 0; i < 3; i++)
        {
            prediction.error[i] = estimate.error[i] + process_noise[i];
        }

        prediction.acceleration = estimate.acceleration;
        prediction.velocity = estimate.velocity + (estimate.acceleration * dt);
        prediction.yaw_rate = estimate.yaw_rate;
        prediction.yaw_position = estimate.yaw_position + (estimate.yaw_rate * dt);

        double shift = (estimate.velocity * dt) + (0.5 * estimate.acceleration * std::pow(dt, 2));
        prediction.x += shift * std::cos(to_radian(estimate.yaw_position));
        prediction.y += shift * std::sin(to_radian(estimate.yaw_position));
        prediction.vx = estimate.velocity * std::cos(to_radian(estimate.yaw_position));
        prediction.vy = estimate.velocity * std::sin(to_radian(estimate.yaw_position));
        prediction.ax = estimate.acceleration * std::cos(to_radian(estimate.yaw_position));
        prediction.ay = estimate.acceleration * std::sin(to_radian(estimate.yaw_position));
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
        default:
        {
            type = "unknown";
        }
        }
        x = object.estimate.position[0] - host.estimate.x;
        y = object.estimate.position[1] - host.estimate.y;
        z = object.estimate.position[2];

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
            data.position[0] += host.estimate.x;
            data.position[1] += host.estimate.y;
            data.velocity[0] += host.estimate.vx;
            data.velocity[1] += host.estimate.vy;
            data.acceleration[0] += host.estimate.ax;
            data.acceleration[1] += host.estimate.ay;
        }

<<<<<<< HEAD
        void update_objects (std::vector<MeasuredState> &sensor_data)
        {
            std::cout << "sensor_data.size(): " << sensor_data.size() << "\n";
            if (!host_ready) return;

            for (auto &data : sensor_data) {
                data.position[0] += host.estimate.x;
                data.position[1] += host.estimate.y;
                data.velocity[0] += host.estimate.vx;
                data.velocity[1] += host.estimate.vy;
                data.acceleration[0] += host.estimate.ax;
                data.acceleration[1] += host.estimate.ay;
            }

            // trying to merge new measurements into existing objects
            // if an object does not have a match, it is most likely in a blind spot
            // if a measurement does not have a match, it is most likely a new object
            std::vector<bool> object_has_match(objects.size(), false), data_has_match(sensor_data.size(), false);
=======
        // trying to merge new measurements into existing objects
        // if an object does not have a match, it is most likely in a blind spot
        // if a measurement does not have a match, it is most likely a new object
        std::vector<bool> object_has_match(objects.size(), false), data_has_match(sensor_data.size(), false);
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce

        for (int i = 0; i < objects.size(); i++)
        {
            for (int j = 0; j < sensor_data.size(); j++)
            {
                if (object_has_match[i] || data_has_match[j])
                {
                    continue;
                }

<<<<<<< HEAD
                    bool match = true;
                    for (int axis = 0; axis < 3; axis++) {
                        double object_pos = objects[i].prediction.position[axis];
                        double new_pos = sensor_data[j].position[axis];
                        double difference = std::abs(new_pos - object_pos);
                        if (difference > MERGE_DISTANCE) {
                            match = false;
                            break;
                        }
=======
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
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce
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

        for (int i = objects.size() - 1; i >= 0; i--)
        {
            if (!object_has_match[i])
            {
                for (auto &u : objects[i].measurement.error)
                {
                    u = INFINITE_ERROR;
                }
                objects[i].timeout++;
            }
<<<<<<< HEAD

            while (true) {
                auto it = std::find_if(objects.begin(), objects.end(), [&] (auto &i) { return i.timeout > TIMEOUT_TICKS; });
                if (it == objects.end()) break;

                objects.erase(it);
=======
            else
            {
                objects[i].timeout = 0;
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce
            }
            objects[i].update();
        }

        while (true)
        {
            auto it = std::find_if(objects.begin(), objects.end(), [&](auto &i)
                                   { return i.timeout > TIMEOUT_TICKS; });
            if (it == objects.end())
                break;
            objects.erase(it);
        }

<<<<<<< HEAD
        void tick (TickData &data)
=======
        for (int i = 0; i < sensor_data.size(); i++)
>>>>>>> 4f2899b27dc9c548bbf434256ce0fc0d6def37ce
        {
            if (!data_has_match[i])
            {
                objects.push_back(Object(sensor_data[i]));
            }
        }
    }

    void tick(const TickData &data)
    {
        time = time + 0.01;

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