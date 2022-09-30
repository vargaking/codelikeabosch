#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

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
<<<<<<< HEAD
    public:
        ObjectType type;
        MeasuredState measurement;
        EstimateState estimate;
        PredictionState prediction;

        Object (MeasuredState initial_state)
=======
public:
    ObjectType type;
    World &world;
    MeasuredState measurement;
    EstimateState estimate;
    PredictionState prediction;

    Object(World &world_, MeasuredState initial_state)
        : world(world_)
    {
        for (int axis = 0; axis < 3; axis++)
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
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
        }
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
}

void
predict(double dt, double process_noise[3])
{
    for (int axis = 0; axis < 3; axis++)
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
>>>>>>> 6b81d130dfd31eb25142b6e158174ddfa75c8445
    }
}
}
;

class HostMotionState
{
public:
    double velocity[2], acceleration[2], yaw_rate;
};

class HostMeasuredState : public HostMotionState
{
<<<<<<< HEAD
    public:
        double error[3];
=======
public:
    double error[2];
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
};

class HostEstimateState : public HostMotionState
{
<<<<<<< HEAD
    public:
        double position[2], gain[3], yaw_position, error[3];
=======

public:
    double position[2], gain[2], error[2];
>>>>>>> 6b81d130dfd31eb25142b6e158174ddfa75c8445
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
};

class HostPredictionState : public HostMotionState
{
<<<<<<< HEAD
    public:
        double position[2], error[3];
=======

public:
    double position[2], error[2];
>>>>>>> 6b81d130dfd31eb25142b6e158174ddfa75c8445
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
};

class HostObject
{
<<<<<<< HEAD
    public:
        HostMeasuredState measurement;
        HostEstimateState estimate;
        HostPredictionState prediction;

        HostObject ();

        HostObject (HostMeasuredState initial_state)
        {
            for (int i = 0; i < 3; i++) {
                estimate.error[i] = measurement.error[i];
            }

            for (int axis = 0; axis < 2; axis++) {
                estimate.yaw_position = 0;
                estimate.position[axis] = 0;
                estimate.yaw_rate = initial_state.yaw_rate;
                estimate.velocity[axis] = initial_state.velocity[axis];
                estimate.acceleration[axis] = initial_state.acceleration[axis];
            }
        }

        void update (HostMeasuredState sensor_data)
        {
            for (int i = 0; i < 3; i++) {
                estimate.gain[i] = prediction.error[i] / (prediction.error[i] + measurement.error[i]);
                estimate.error[i] = (1 - estimate.gain[i]) * prediction.error[i];
            }

            for (int axis = 0; axis < 2; axis++) {
                estimate.position[axis] = prediction.position[axis];
                estimate.yaw_rate = prediction.yaw_rate;
                estimate.velocity[axis] = prediction.velocity[axis] + estimate.gain[0] * (measurement.velocity[axis] - prediction.velocity[axis]);
                estimate.acceleration[axis] = prediction.acceleration[axis] + estimate.gain[1] * (measurement.acceleration[axis] - prediction.acceleration[axis]);
            }

            estimate.yaw_rate = prediction.yaw_rate + estimate.gain[2] * (measurement.yaw_rate - prediction.yaw_rate);
        }

        void predict (double dt, double process_noise[2])
        {
            for (int i = 0; i < 3; i++) {
                prediction.error[i] = estimate.error[i] + process_noise[i];
            }

            for (int axis = 0; axis < 2; axis++) {
                prediction.acceleration[axis] = estimate.acceleration[axis];
                prediction.velocity[axis] = estimate.velocity[axis] + (estimate.acceleration[axis] * dt);
                prediction.position[axis] = estimate.position[axis] + (estimate.velocity[axis] * dt) + (0.5 * estimate.acceleration[axis] * std::pow(dt, 2));
            }

            prediction.yaw_rate = estimate.yaw_rate + (estimate.yaw_rate * dt);
=======
public:
    HostMeasuredState measurement;
    HostEstimateState estimate;
    HostPredictionState prediction;

    void update(HostMeasuredState sensor_data)
    {
        for (int i = 0; i < 2; i++)
        {
            estimate.gain[i] = prediction.error[i] / (prediction.error[i] + measurement.error[i]);
            estimate.error[i] = (1 - estimate.gain[i]) * prediction.error[i];
        }

        for (int axis = 0; axis < 3; axis++)
        {
            // estimate.position[axis] = prediction.position[axis] + estimate.gain[0] * (measurement.position[axis] - prediction.position[axis]);
            estimate.velocity[axis] = prediction.velocity[axis] + estimate.gain[1] * (measurement.velocity[axis] - prediction.velocity[axis]);
            estimate.acceleration[axis] = prediction.acceleration[axis] + estimate.gain[2] * (measurement.acceleration[axis] - prediction.acceleration[axis]);
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
        }
    }

    void predict()
    {
    }
};

class TickData
{
<<<<<<< HEAD
    public:
        bool is_host_updated;
        HostMeasuredState host_state;
        std::vector<MeasuredState> object_states;
=======
public:
    bool is_host_updated;
    HostMeasuredState host_state;
    std::vector<MeasuredState> object_states;
    double timestamp;
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
};
>>>>>>> 6b81d130dfd31eb25142b6e158174ddfa75c8445

class World
{
<<<<<<< HEAD
    private:
        HostObject host;
        bool host_ready;
        std::vector<Object> objects;
        int time;

    public:
        World ()
        {
            time = 0;
            host_ready = false;
        }

        void update_objects (std::vector<MeasuredState> sensor_data)
        {
            if (!host_ready) return;

            for (auto &data : sensor_data) {
                for (int axis = 0; axis < 3; axis++) {
                    data.position[axis] += host.prediction.position[axis];
                }
=======
private:
    HostObject host;
    std::vector<Object> objects;
    double last_timestamp;

public:
    void update_objects(std::vector<MeasuredState> sensor_data, double timestamp)
    {
        for (auto &data : sensor_data)
        {
            for (int axis = 0; axis < 3; axis++)
            {
                data.position[axis] += host.estimate.position[axis];
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
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

<<<<<<< HEAD
            for (int i = 0; i < sensor_data.size(); i++) {
                if (!data_has_match[i]) {
                    objects.push_back(Object(sensor_data[i]));
=======
        for (int i = 0; i < objects.size(); i++)
        {
            if (!object_has_match[i])
            {
                for (auto &u : objects[i].measurement.error)
                {
                    u = INFINITE_ERROR;
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
                }
            }
        }

        for (int i = 0; i < sensor_data.size(); i++)
        {
<<<<<<< HEAD
            if (!host_ready) {
                if (data.is_host_updated) {
                    host = HostObject(data.host_state);
                    host_ready = true;
                    double process_noise[3] = {1, 1, 1};
                    host.predict(time + 0.01, process_noise);
                }
            } else {
                if (data.is_host_updated) {
                    host.update(data.host_state);
                }

                update_objects(data.object_states);
=======

            if (!data_has_match[i])
            {
                objects.push_back(Object(*this, sensor_data[i]));
            }

            if (data.is_host_updated)
            {
                host.update(data.host_state);
            }
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5

                double process_noise[3] = {1, 1, 1};

<<<<<<< HEAD
                host.predict(time + 0.01, process_noise);

                for (auto &object : objects) {
                    object.predict(time + 0.01, process_noise);
                }
=======
            host.predict();
            double process_noise[3] = {1, 1, 1};
            for (auto &object : objects)
            {
                object.predict(data.timestamp - last_timestamp, process_noise);
>>>>>>> 4d2570cb8cd8de83c8d403a536cd7b70d74eede5
            }

            time += 0.01;
        }
    }

    void tick(const TickData &data)
    {
        host.update(data.);
        update_objects(data.object_states, data.timestamp);
    }
};