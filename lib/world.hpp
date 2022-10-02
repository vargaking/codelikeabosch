// see README.md

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>

#include "collision.hpp"

#define INFINITE_ERROR 1000000
#define DATA_MERGE_DISTANCE 2
#define OBJECT_MERGE_DISTANCE 2
#define TIMEOUT_TICKS 1000

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
        double gain[3], error[3], angle;
};

class PredictionState : public MotionState
{
    public:
        double error[3], angle;
};

class Object
{
    public:
        ObjectType type;
        MeasuredState measurement;
        EstimateState estimate;
        PredictionState prediction;
        int id;
        int timeout;

        Object (MeasuredState &initial_state)
        {
            for (int i = 0; i < 3; i++) {
                estimate.error[i] = measurement.error[i];
            }

            for (int axis = 0; axis < 3; axis++) {
                estimate.position[axis] = initial_state.position[axis];
                estimate.velocity[axis] = initial_state.velocity[axis];
                estimate.acceleration[axis] = initial_state.acceleration[axis];
            }

            std::random_device dev;
            std::mt19937 rng(dev());
            id = rng();

            estimate.angle = 0;

            timeout = 0;
        }

        void update ()
        {   
            if (measurement.type != ObjectType::unknown) {
                type = measurement.type;
            }

            for (int i = 0; i < 3; i++) {
                estimate.gain[i] = prediction.error[i] / (prediction.error[i] + measurement.error[i]);
                estimate.error[i] = (1 - estimate.gain[i]) * prediction.error[i];
            }

            for (int axis = 0; axis < 3; axis++) {
                estimate.position[axis] = prediction.position[axis] + estimate.gain[0] * (measurement.position[axis] - prediction.position[axis]);
                estimate.velocity[axis] = prediction.velocity[axis] + estimate.gain[1] * (measurement.velocity[axis] - prediction.velocity[axis]);
                estimate.acceleration[axis] = prediction.acceleration[axis] + estimate.gain[2] * (measurement.acceleration[axis] - prediction.acceleration[axis]);
            }

            estimate.angle = prediction.angle;
        }

        void predict (double dt, double process_noise[3])
        {
            for (int i = 0; i < 3; i++) {
                prediction.error[i] = estimate.error[i] + process_noise[i];
            }

            for (int axis = 0; axis < 3; axis++) {
                prediction.acceleration[axis] = estimate.acceleration[axis];
                prediction.velocity[axis] = estimate.velocity[axis] + (estimate.acceleration[axis] * dt);
                prediction.position[axis] = estimate.position[axis] + (estimate.velocity[axis] * dt) + (0.5 * estimate.acceleration[axis] * std::pow(dt, 2));
            }

            double angle_target = std::atan(estimate.velocity[1] / estimate.velocity[0]);
            double abs_velocity = std::sqrt(std::pow(estimate.velocity[0], 2) + std::pow(estimate.velocity[1], 2));

            prediction.angle = estimate.angle + std::min(abs_velocity / 2, 1.0) * (angle_target - estimate.angle);
        }

        Hitbox get_hitbox ()
        {
            double front, back, side;

            switch (type) {
                case ObjectType::car: {
                    front = 3.9;
                    back = 0.9;
                    side = 1;
                    break;
                }
                case ObjectType::truck: {
                    front = 8.5;
                    back = 3;
                    side = 1.25;
                    break;
                }
                case ObjectType::bicycle: {
                    front = 1.3;
                    back = 0.8;
                    side = 0.5;
                    break;
                }
                case ObjectType::car_or_truck: {
                    front = 6;
                    back = 2;
                    side = 1.2;
                    break;
                }
                case ObjectType::pedestrian: {
                    front = 0.15;
                    back = 0.15;
                    side = 0.4;
                    break;
                }
                case ObjectType::motorbike: {
                    front = 1.4;
                    back = 0.9;
                    side = 0.55;
                    break;
                }
                default: {
                    front = 3.9;
                    back = 0.9;
                    side = 1;
                    break;
                }
            }

            return Hitbox(estimate.position[0], estimate.position[1], estimate.angle, front, back, side);
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

        HostObject () = default;

        HostObject (HostMeasuredState &initial_state)
        {
            for (int i = 0; i < 3; i++) {
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

        void update ()
        {
            for (int i = 0; i < 3; i++) {
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

        void predict (double dt, double process_noise[2])
        {
            for (int i = 0; i < 3; i++) {
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

        TickData ()
        {
            is_host_updated = false;
        }
};

class ObjectSnapshot {
    public:
        std::string type;
        double x, y, z;
        double angle;
        int id;

        ObjectSnapshot (HostObject &host, Object &object)
        {
            switch (object.type) {
                case ObjectType::unknown: {
                    type = "unknown"; break;
                }
                case ObjectType::truck: {
                    type = "truck"; break;
                }
                case ObjectType::car: {
                    type = "car"; break;
                }
                case ObjectType::motorbike: {
                    type = "motorbike"; break;
                }
                case ObjectType::bicycle: {
                    type = "bicycle"; break;
                }
                case ObjectType::pedestrian: {
                    type = "pedestrian"; break;
                }
                case ObjectType::car_or_truck: {
                    type = "car_or_truck"; break;
                }
                default: {
                    type = "unknown";
                }
            }
            x = object.estimate.position[0] - host.estimate.x;
            y = - object.estimate.position[1] + host.estimate.y;
            angle = - object.estimate.angle;

            id = object.id;
        }

        ObjectSnapshot (HostObject &host)
        {
            type = "host";
            x = host.estimate.x;
            y = - host.estimate.y;
            angle = - host.estimate.yaw_position;
            id = 0;
        }

        std::string get_type () { return type; }
        double get_x () { return x; }
        double get_y () { return y; }
        double get_z () { return z; }
        int get_id () { return id; }
        double get_angle () { return angle; }
};

double distance (Object &p, Object &q) 
{
    double dx = std::abs(p.prediction.position[0] - q.prediction.position[0]);
    double dy = std::abs(p.prediction.position[1] - q.prediction.position[1]);
    double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    //std::cout << dx << " " << dy << " " << distance << "\n";
    return distance;
}

double distance (Object &p, MeasuredState &q)
{
    double dx = std::abs(p.prediction.position[0] - q.position[0]);
    double dy = std::abs(p.prediction.position[1] - q.position[1]);
    double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    return distance;
}

class World 
{
    public:
        HostObject host;
        bool host_ready;
        std::vector<Object> objects;
        double time;
        bool warning;

    public:
        World ()
        {
            time = 0;
            host_ready = false;
            warning = false;
        }

        void update_objects (std::vector<MeasuredState> &sensor_data)
        {
            //std::cout << "sensor_data.size(): " << sensor_data.size() << "\n";
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

            for (int i = 0; i < objects.size(); i++) {
                for (int j = 0; j < sensor_data.size(); j++) {
                    if (data_has_match[j]) {
                        continue;
                    }

                    if (distance(objects[i], sensor_data[j]) < DATA_MERGE_DISTANCE) {
                        object_has_match[i] = true;
                        data_has_match[j] = true;
                        objects[i].measurement = sensor_data[j];
                    }
                }
            }

            for (int i = objects.size() - 1; i >= 0; i--) {
                if (!object_has_match[i]) {
                    for (auto &u : objects[i].measurement.error) {
                        u = INFINITE_ERROR;
                    }
                    // incrementally increasing
                    objects[i].timeout++;
                } else {
                    // steeply decreasing
                    objects[i].timeout -= 10;
                }
                objects[i].update();
            }

            while (true) {
                auto it = std::find_if(objects.begin(), objects.end(), [&] (auto &i) { return i.timeout > TIMEOUT_TICKS; });
                if (it == objects.end()) break;
                objects.erase(it);
                //std::cout << "deleting\n";
            }

            //std::cout << objects.size() << "\n";

            for (int i = 0; i < sensor_data.size(); i++) {
                if (!data_has_match[i]) {
                    objects.push_back(Object(sensor_data[i]));
                }
            }
        }

        void merge_objects ()
        {
            while (true) {
                bool found = false;
                int index;
                for (int i = 0; i < objects.size(); i++) {
                    for (int j = i + 1; j < objects.size(); j++) {
                        //std::cout << "dist: " << distance(objects[i], objects[j]) << "\n";
                        if (distance(objects[i], objects[j]) < OBJECT_MERGE_DISTANCE) {
                            index = j;
                            found = true;
                            if (objects[i].type == ObjectType::unknown) {
                                objects[i].type = objects[j].type;
                            };
                            goto break_;
                        }
                    }
                }
                break_:
                if (found) {
                    objects.erase(objects.begin() + index);
                } else {
                    break;
                }
            }
        }

        void tick (TickData &data)
        {
            time = time + 0.01;

            if (!host_ready) {
                if (data.is_host_updated) {
                    host = HostObject(data.host_state);
                    host_ready = true;
                    double process_noise[3] = {1, 1, 1};
                    host.predict(0.01, process_noise);
                }
            } else {
                //std::cout << objects.size() << "\n";

                merge_objects();

                //std::cout << objects.size() << "\n";

                if (data.is_host_updated) {
                    host.measurement = data.host_state;
                    host.update();
                } else {
                    HostMeasuredState h;
                    for (auto &i : h.error) {
                        i = INFINITE_ERROR;
                    }
                    host.measurement = h;
                    host.update();
                }

                std::cout << objects.size() << "\n";

                double process_noise[3] = {1, 1, 1};

                host.predict(0.01, process_noise);

                update_objects(data.object_states);

                for (auto &object : objects) {
                    object.predict(0.01, process_noise);
                }

                warning = check_host_sides();
                //std::cout << warning << "\n";
            }
        }

        bool check_host_sides ()
        {
            double angle = to_radian(host.estimate.yaw_position);
            double sin = std::sin(angle);
            double cos = std::cos(angle);

            auto left = Hitbox(host.estimate.x + cos * 0.85 - sin * 1.55, host.estimate.y - sin * 0.85 - cos * 1.55, host.estimate.yaw_position, 1.15, 1.15, 0.75);
            auto right = Hitbox(host.estimate.x + cos * 0.85 + sin * 1.55, host.estimate.y - sin * 0.85 + cos * 1.55, host.estimate.yaw_position, 1.15, 1.15, 0.75);

            bool collision = false;
            for (auto &object : objects) {
                Hitbox h = object.get_hitbox();
                collision |= h.check_collision(left);
                collision |= h.check_collision(right);
            }
            
            return collision;
        }

        std::vector<ObjectSnapshot> export_objects ()
        {
            std::vector<ObjectSnapshot> result;
            //if (host_ready) result.push_back(ObjectSnapshot(host));
            for (auto &object : objects) {
                result.push_back(ObjectSnapshot(host, object));
            }
            return result;
        }
};