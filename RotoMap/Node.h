#pragma once
#include "Sensor.h"

struct Node {
    Node* next;
    Sensor* sensor;

    Node() : next(nullptr), sensor(nullptr) {}
    Node(Sensor* value) : sensor(value), next(nullptr) {}
    Node(Sensor* value, Node* nextNode) : sensor(value), next(nextNode) {}
};

