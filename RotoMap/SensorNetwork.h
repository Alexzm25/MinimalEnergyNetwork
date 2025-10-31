#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "WeightedGraph.h"
#include "Sensor.h"

using namespace std;

class SensorNetwork {
private:
    WeightedGraph graph;

public:
    SensorNetwork() {}

    ~SensorNetwork() {
        graph.clear();
    }

    void addSensor(const string& name, float x, float y) {
        Sensor* newSensor = new Sensor(x, y, name);
        graph.addSensor(newSensor);
    }

    void generateExampleSensors() {
        addSensor("Sensor_A", 10.0f, 20.0f);
        addSensor("Sensor_B", 40.0f, 50.0f);
        addSensor("Sensor_C", 80.0f, 30.0f);
        addSensor("Sensor_D", 60.0f, 70.0f);
        addSensor("Sensor_E", 30.0f, 80.0f);
        addSensor("Sensor_F", 90.0f, 90.0f);
    }

    void buildCompleteNetwork() {
        graph.generateCompleteGraph();
    }

    bool loadFromFile(const string& filename) {
        return graph.loadSensorsFromFile(filename);
    }

    bool saveToFile(const string& filename) const {
        return graph.saveSensorsToFile(filename);
    }

    WeightedGraph& getGraph() {
        return graph;
    }
};
