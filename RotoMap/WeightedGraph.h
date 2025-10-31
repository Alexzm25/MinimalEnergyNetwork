#pragma once
#include <vector>
#include <utility>
#include "Sensor.h"

struct Edge {
    Sensor* from;
    Sensor* to;
    float weight;
    Edge(Sensor* f, Sensor* t, float w) : from(f), to(t), weight(w) {}
};

class WeightedGraph {
private:
    vector<Sensor*> sensors;
    vector<Edge> edges;
public:
    void addSensor(Sensor* sensor) {
        sensors.push_back(sensor);
    }
    void addConnection(Sensor* from, Sensor* to, float weight) {
        edges.emplace_back(from, to, weight);
    }
    const vector<Sensor*>& getSensors() const {
        return sensors;
    }
    const vector<Edge>& getEdges() const {
        return edges;
    }
};
